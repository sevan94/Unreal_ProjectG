#include "DataAssets/Items/DataAsset_ArmorData.h"
#include "AbilitySystem/Effects/GEffect_SetByCallerAdditive.h"
#include "AbilitySystem/Effects/GEffect_SetByCallerMultiply.h"
#include "AbilitySystem/PGAbilitySystemComponent.h"
#include "PGFunctionLibrary.h"

void UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle(UPGAbilitySystemComponent* InASC, int32 InLevel) const
{
    if (AttributeModifiers.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle - No attribute modifiers defined for this armor data asset."));
        return;
    }

    TArray<const FPGAttributeModifierEntry*> AdditiveModifiers;
    TArray<const FPGAttributeModifierEntry*> MultiplicativeModifiers;

    // AttributeModifiers를 ModifierOp에 따라 분류
    for (const FPGAttributeModifierEntry& Entry : AttributeModifiers)
    {
        // 해당 어트리뷰트에 매핑된 SetByCaller가 없으면 스킵
        if(!UPGFunctionLibrary::GetSetByCallerTagForAttribute(Entry.Attribute).IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle - No valid SetByCaller tag found for attribute %s. Skipping this modifier."), *Entry.Attribute.GetName());
            continue;
        }
        switch (Entry.ModifierOp)
        {
        case EPGEffectModOp::Additive:
            AdditiveModifiers.Add(&Entry);
            break;
        case EPGEffectModOp::MultiplyAdditive:
            MultiplicativeModifiers.Add(&Entry);
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("UDataAsset_ArmorData::MakeOutgoingArmorEffectSpecHandle - Unsupported ModifierOp for attribute %s. Skipping this modifier."), *Entry.Attribute.GetName());
            break;
        }
    }

    // 이펙트 컨텍스트와 스펙 핸들을 생성
    FGameplayEffectContextHandle EffectContext = InASC->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    // Additive 모디파이어 적용
    if (AdditiveModifiers.Num() > 0)
    {
        FGameplayEffectSpecHandle AdditiveSpecHandle = InASC->MakeOutgoingSpec(UGEffect_SetByCallerAdditive::StaticClass(), InLevel, EffectContext);

        if (AdditiveSpecHandle.IsValid())
        {
            for (const FPGAttributeModifierEntry* Entry : AdditiveModifiers)
            {
                FGameplayTag DataTag = UPGFunctionLibrary::GetSetByCallerTagForAttribute(Entry->Attribute);
                AdditiveSpecHandle.Data->SetSetByCallerMagnitude(DataTag, Entry->ModifierValue.GetValueAtLevel(InLevel));
            }
            InASC->ApplyGameplayEffectSpecToSelf(*AdditiveSpecHandle.Data);
        }
    }

    // Multiplicative 모디파이어 적용
    if (MultiplicativeModifiers.Num() > 0)
    {
        FGameplayEffectSpecHandle MultiplicativeSpecHandle = InASC->MakeOutgoingSpec(UGEffect_SetByCallerMultiply::StaticClass(), InLevel, EffectContext);

        if (MultiplicativeSpecHandle.IsValid())
        {
            for (const FPGAttributeModifierEntry* Entry : MultiplicativeModifiers)
            {
                FGameplayTag DataTag = UPGFunctionLibrary::GetSetByCallerTagForAttribute(Entry->Attribute);
                MultiplicativeSpecHandle.Data->SetSetByCallerMagnitude(DataTag, Entry->ModifierValue.GetValueAtLevel(InLevel));
            }
            InASC->ApplyGameplayEffectSpecToSelf(*MultiplicativeSpecHandle.Data);
        }
    }
}
