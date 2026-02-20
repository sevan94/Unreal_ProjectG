#include "AbilityEntryCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Types/PGStructTypes.h"
#include "DataAssets/Ability/AbilityConfig.h"

// 어빌리티 클래스 헤더
#include "AbilitySystem/Abilities/Player/HeroAbility_BaseMeleeAttack.h"
#include "AbilitySystem/Abilities/Player/HeroAbility_BaseProjectileAttack.h"

// 어빌리티 클래스 -> Config 클래스 매핑 테이블
// 새 어빌리티 혹은 Config 추가 시 이곳에 추가
static TMap<UClass*, UClass*> GetAbilityToConfigMap()
{
    TMap<UClass*, UClass*> Map;
    Map.Add(UHeroAbility_BaseMeleeAttack::StaticClass(), UMeleeAttackAbilityConfig::StaticClass());
    Map.Add(UHeroAbility_BaseProjectileAttack::StaticClass(), USpawnProjectileAbilityConfig::StaticClass());

    return Map;
}

// 헤더 부분 커스터마이징
void FAbilityEntryCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& Utils)
{
    HeaderRow.NameContent()
    [
        PropertyHandle->CreatePropertyNameWidget()
        // 글씨 색상 변경 등
        //SNew(STextBlock)
        //.Text(FText::FromString("커스텀 이름"))
        //.ColorAndOpacity(FLinearColor::Red)
    ];
}

// 자식 부분 커스터마이징
void FAbilityEntryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& Utils)
{
    // 패널 업데이트용 Utils 저장
    PropertyUtilities = Utils.GetPropertyUtilities();

    // FAbilityEntry의 AbilityClass와 AbilityConfig 핸들 가져오기
    AbilityClassHandle = PropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FAbilityEntry, AbilityClass));
    AbilityConfigHandle = PropertyHandle->GetChildHandle(
        GET_MEMBER_NAME_CHECKED(FAbilityEntry, AbilityConfig));

    // 진입점에서 현재 AbilityClass에 맞는 Config 클래스 필터링
    UpdateConfigClassFilter();

    // AbilityClass 변경 감지 -> Config 초기화
     AbilityClassHandle->SetOnPropertyValueChanged(
        FSimpleDelegate::CreateSP(this, &FAbilityEntryCustomization::OnAbilityClassChanged)); // Create SP는 SharedFromThis 기반이라 객체가 소멸되면 자동으로 델리게이트 실행을 멈춤
    //AbilityClassHandle->SetOnPropertyValueChanged(
        //FSimpleDelegate::CreateRaw(this, &FAbilityEntryCustomization::OnAbilityClassChanged));

    // 어빌리티 클래스는 항상 표시
    ChildBuilder.AddProperty(AbilityClassHandle.ToSharedRef());
    
    ChildBuilder.AddProperty(AbilityConfigHandle.ToSharedRef())
        .Visibility(TAttribute<EVisibility>::Create(
            TAttribute<EVisibility>::FGetter::CreateSP(
                this, &FAbilityEntryCustomization::GetConfigVisibility))); // AbilityClass에 맞는 Config가 있을 때만 표시
}

void FAbilityEntryCustomization::UpdateConfigClassFilter()
{
    if (!AbilityConfigHandle.IsValid() || !AbilityConfigHandle->IsValidHandle()) return;

    FProperty* ConfigProperty = AbilityConfigHandle->GetProperty();
    if (!ConfigProperty) return;

    UClass* RequiredClass = GetRequiredConfigClass(); // 못 찾으면 UAbilityConfig 반환

    UE_LOG(LogTemp, Warning, TEXT("Required Config Class: %s"), *RequiredClass->GetName());

    // AllowedClasses 메타데이터를 동적으로 변경하여 에디터 클래스 피커가 이 값을 참조
    ConfigProperty->SetMetaData(TEXT("AllowedClasses"), *RequiredClass->GetPathName());
}

void FAbilityEntryCustomization::OnAbilityClassChanged()
{
    // IsValid()는 포인터가 NUllL이 아닌지만 체크, IsValidHandle()은 핸들이 유효한지 체크
    if (!AbilityConfigHandle.IsValid() || !AbilityConfigHandle->IsValidHandle()) return;
    if (!AbilityClassHandle.IsValid() || !AbilityClassHandle->IsValidHandle()) return;

    UObject* CurrentAbilityConfig = nullptr;
    AbilityConfigHandle->GetValue(CurrentAbilityConfig);

    if (CurrentAbilityConfig)
    {
        //새로 선택된 어빌리티 클래스에 매핑되는 Config 클래스 찾기
        UClass* RequiredConfigClass = GetRequiredConfigClass();

        // Config 타입이 맞지 않으면 Config 초기화
        if (RequiredConfigClass && !CurrentAbilityConfig->IsA(RequiredConfigClass))
        {
            TSharedPtr<IPropertyHandle> LocalHandle = AbilityConfigHandle; // 람다 캡처용 로컬 변수
            GEditor->GetTimerManager()->SetTimerForNextTick([LocalHandle]()
            {
                if (LocalHandle.IsValid())
                {
                    //AbilityConfigHandle->SetValue((UObject*)nullptr);
                    LocalHandle->SetValueFromFormattedString(TEXT("None"));
                }
            });
        }
    }

    // 디테일 패널 업데이트
    if(PropertyUtilities.IsValid())
    {
        PropertyUtilities->RequestRefresh();
    }
}

UClass* FAbilityEntryCustomization::GetRequiredConfigClass() const
{
    if (!AbilityClassHandle.IsValid()) return UAbilityConfig::StaticClass();

    //현재 선택된 AbilityClass 가져오기
    UObject* AbilityClassObj = nullptr;
    AbilityClassHandle->GetValue(AbilityClassObj);
    UClass* AbilityClass = Cast<UClass>(AbilityClassObj);

    if (!AbilityClass) return UAbilityConfig::StaticClass();

    // 매핑 테이블에서 Config 클래스 찾기
    const TMap<UClass*, UClass*> AbilityToConfigMap = GetAbilityToConfigMap();

    // 정확한 클래스 또는 부모 클래스 기준으로 검색
    for(const auto& Pair : AbilityToConfigMap)
    {
        if (AbilityClass->IsChildOf(Pair.Key))
        {
            return Pair.Value;
        }
    }
    return UAbilityConfig::StaticClass();
}

EVisibility FAbilityEntryCustomization::GetConfigVisibility() const
{
    // AbilityClass가 유효하지 않으면 Config 숨김
    if (!AbilityClassHandle.IsValid()) return EVisibility::Collapsed;
    
    UObject* AbilityClassObj = nullptr;
    AbilityClassHandle->GetValue(AbilityClassObj);

    // 어빌리티 클래스에 매핑되는 Config가 있다면 표시, 없으면 숨김
    return AbilityClassObj ? EVisibility::Visible : EVisibility::Collapsed;
}