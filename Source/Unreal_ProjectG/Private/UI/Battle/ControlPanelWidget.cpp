// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/ControlPanelWidget.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Hero/HeroCharacter.h"
#include "Character/HeroController.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystem/PGCharacterAttributeSet.h"
#include "UI/Battle/BarWidget.h"
#include "UI/Battle/BaseHpWidget.h"
#include "UI/Battle/UnitPanelWidget.h"
#include "UI/Battle/ActiveSkillWidget.h"
#include "Interfaces/JoysticInput.h"
#include "Kismet/GameplayStatics.h"
#include "Pawn/BaseStructure.h"
#include "Components/Equipment/EquipmentsStorageComponent.h"
#include "Mode/Save/PGGameInstance.h"
#include "DataAssets/UI/EquipUIDataAsset.h"

void UControlPanelWidget::SetAbilitySpecHandle()
{
    UPGGameInstance* GI = Cast<UPGGameInstance>(GetGameInstance());
    TArray<FGameplayAbilitySpecHandle> SpecHandleArray = HeroCharacter->GetEquipmentsStorageComponent()->GetSkillAbilitySpecHandles();
    if (!SpecHandleArray.IsEmpty())
    {
        //UE_LOG(LogTemp, Log, TEXT("스펙 핸들 가져옴"));
        if(SpecHandleArray[0].IsValid())WeaponSkill1->SetAbilitySpecHandle(SpecHandleArray[0]);
        //if(SpecHandleArray[1].IsValid())WeaponSkill2->SetAbilitySpecHandle(SpecHandleArray[1]);
        if (GI->CurrentWeapon)
        {
            WeaponSkill1->SetSkillIcon(GI->CurrentWeapon->SkillImage1);
            WeaponSkill2->SetSkillIcon(GI->CurrentWeapon->SkillImage2);
        }
    }
}

void UControlPanelWidget::UpdateHeroHP(float InValue)
{
    HPBar->UpdateCurrent(InValue);
}

void UControlPanelWidget::UpdateMaxHeroHP(float InValue)
{
    HPBar->UpdateMax(InValue);
}

void UControlPanelWidget::UpdateCost(float InValue)
{
    CostBar->UpdateCurrent(InValue);
    UnitPanel->UpdateAllSlots(InValue);
}

void UControlPanelWidget::UpdateMaxCost(float InValue)
{
    CostBar->UpdateMax(InValue);
}

void UControlPanelWidget::UpdateBaseHP(FGameplayTag TeamTag, float InValue)
{
    if (TeamTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Ally"))))
    {
        PlayerHP->UpdateCurrentHP(InValue);
    }
    else if (TeamTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Foe"))))
    {
        EnemyHP->UpdateCurrentHP(InValue);
    }
}

void UControlPanelWidget::UpdateBaseMaxHP(FGameplayTag TeamTag, float InValue)
{
    if (TeamTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Ally"))))
    {
        PlayerHP->UpdateMaxHP(InValue);
    }
    else if (TeamTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Unit.Side.Foe"))))
    {
        EnemyHP->UpdateMaxHP(InValue);
    }
}

void UControlPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 영웅 캐릭터 초기화
    Controller = Cast<AHeroController>(GetOwningPlayer());

    HeroCharacter = Cast<AHeroCharacter>(GetOwningPlayerPawn());

    // 영웅 및 기지 바인딩
    BindBase();
    BindHero();

    if (HPIcon && CostIcon)
    {
        HPBar->InitProgressBar(HPIcon, FLinearColor::Red, FText::FromString(TEXT("Hero HP")));
        CostBar->InitProgressBar(CostIcon, FLinearColor::Blue, FText::FromString(TEXT("Cost")));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("아이콘이 없음"));
    }
}

FReply UControlPanelWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (JoyStickBackground)
    {
        // 마우스의 스크린 좌표 가져오기
        FVector2D ScreenPos = InMouseEvent.GetScreenSpacePosition();

        // 조이스틱 배경의 지오메트리 정보 가져오기
        const FGeometry& BackgroundGeometry = JoyStickBackground->GetCachedGeometry();

        // 해당 배경 영역 안에 마우스가 있는지 체크
        if (BackgroundGeometry.IsUnderLocation(ScreenPos))
        {
            bIsAreaPressed = true;

            // 위젯 로컬 좌표 계산
            FVector2D LocalPos = BackgroundGeometry.AbsoluteToLocal(ScreenPos);
            FVector2D Center = BackgroundGeometry.GetLocalSize() / 2.0f;

            // 중심으로부터의 벡터 계산
            FVector2D Delta = LocalPos - Center;

            // 거리 제한 및 정규화
            float Distance = FMath::Clamp(Delta.Size(), 0.0f, JoystickRange);
            JoystickVector = Delta.GetSafeNormal(); // 이동 방향
            
            //인터페이스를 사용해 캐릭터 움직임
            if(Controller)
            {
                IJoysticInput::Execute_MoveStart(Controller, JoystickVector);
            }

            return FReply::Handled().CaptureMouse(TakeWidget());
        }
    }
    return FReply::Unhandled();
}

FReply UControlPanelWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsAreaPressed)
    {
        // 마우스의 스크린 좌표 가져오기
        FVector2D ScreenPos = InMouseEvent.GetScreenSpacePosition();

        // 2. 조이스틱 배경의 지오메트리 정보 가져오기
        const FGeometry& BackgroundGeometry = JoyStickBackground->GetCachedGeometry();

        // 위젯 로컬 좌표 계산
        FVector2D LocalPos = BackgroundGeometry.AbsoluteToLocal(ScreenPos);
        FVector2D Center = BackgroundGeometry.GetLocalSize() / 2.0f;

        // 중심으로부터의 벡터 계산
        FVector2D Delta = LocalPos - Center;

        // 거리 제한 및 정규화
        float Distance = FMath::Clamp(Delta.Size(), 0.0f, JoystickRange);
        JoystickVector = Delta.GetSafeNormal(); // 이동 방향

        // 조이스틱 이미지 이동
        if (JoyStick)
        {
            JoyStick->SetRenderTranslation(JoystickVector * Distance);
        }

        // 실제 이동에 사용할 벡터 업데이트 (0~1 사이 값)
        JoystickVector *= (Distance / JoystickRange);

        //인터페이스를 사용해 캐릭터 움직임
        if (Controller)
        {
            IJoysticInput::Execute_ChangeDirection(Controller, JoystickVector);
        }

        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply UControlPanelWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsAreaPressed)
    {
        bIsAreaPressed = false;
        JoystickVector = FVector2D::ZeroVector;

        // 조이스틱 위치 초기화
        if (JoyStick)
        {
            JoyStick->SetRenderTranslation(FVector2D::ZeroVector);
        }

        //인터페이스를 사용해 캐릭터 정지
        if (Controller)
        {
            IJoysticInput::Execute_EndMovement(Controller);
        }

        // 마우스 캡처 해제
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}

void UControlPanelWidget::BindHero()
{
    if (HeroCharacter)
    {
        HeroCharacter->OnHeroHpChanged.AddDynamic(this, &UControlPanelWidget::UpdateHeroHP);
        HeroCharacter->OnHeroMaxHpChanged.AddDynamic(this, &UControlPanelWidget::UpdateMaxHeroHP);
        HeroCharacter->OnHeroCostChanged.AddDynamic(this, &UControlPanelWidget::UpdateCost);
        HeroCharacter->OnHeroMaxCostChanged.AddDynamic(this, &UControlPanelWidget::UpdateMaxCost);

        // 영웅 무기 스킬 어빌리티 설정
        TArray<FGameplayAbilitySpecHandle> SpecHandleArray = HeroCharacter->GetEquipmentsStorageComponent()->GetSkillAbilitySpecHandles();
        if (!SpecHandleArray.IsEmpty())
    }
}

void UControlPanelWidget::BindBase()
{
    TArray<AActor*> FoundBases;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseStructure::StaticClass(), FoundBases);

    for (AActor* BaseActor : FoundBases)
    {
        ABaseStructure* Base = Cast<ABaseStructure>(BaseActor);
        if (Base)
        {
            // 기지의 델리게이트에 UI 업데이트 함수 연결
            Base->OnBaseHpChanged.AddDynamic(this, &UControlPanelWidget::UpdateBaseHP);
            Base->OnBaseMaxHpChanged.AddDynamic(this, &UControlPanelWidget::UpdateBaseMaxHP);

            // 초기 값 설정을 위해 수동으로 한 번 호출
            UpdateBaseHP(Base->GetTeamTag(), Base->GetPGCharacterAttributeSet()->GetHealth());
            UpdateBaseMaxHP(Base->GetTeamTag(), Base->GetPGCharacterAttributeSet()->GetMaxHealth());
        }
    }
}
