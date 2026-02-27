#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "AbilitySystem/Effects/GEffect_MasterArmorStatus.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "PGFunctionLibrary.h"

void UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle(UPGAbilitySystemComponent* InASC, int32 InLevel) const
{
    if (AttributeModifiers.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle - No attribute modifiers defined for this armor data asset."));
        return;
    }

    // 이펙트 컨텍스트와 스펙 핸들을 생성
    FGameplayEffectContextHandle EffectContext = InASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    // 오너의 InASC에 Effect를 적용하기 위한 스펙 핸들 생성
    FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(UGEffect_MasterArmorStatus::StaticClass(), InLevel, EffectContext);

    if (SpecHandle.IsValid())
    {
        for (const FArmorModifierEntry& Entry : AttributeModifiers)
        {
            // 해당 Attribute에 매핑된 SetByCaller 태그를 가져옴
            FGameplayTag DataTag = UPGFunctionLibrary::GetSetByCallerTagForAttribute(Entry.Attribute);

            if (DataTag.IsValid())
            {
                SpecHandle.Data->SetSetByCallerMagnitude(
                    DataTag, 
                    Entry.ModifierValue.GetValueAtLevel(InLevel)
                );
            }
        }
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
    }
}
