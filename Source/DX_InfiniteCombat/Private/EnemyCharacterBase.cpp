// Copyright DX_IC


#include "EnemyCharacterBase.h"
#include "ICAbilitySystemComponent.h"
#include "CombatCharacterComponent.h"
#include "DXWidgetBase.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterAttributeSet.h"
#include "ICWorldSubsystem.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UICAbilitySystemComponent>(TEXT("asc"));
	CombatComp = CreateDefaultSubobject<UCombatCharacterComponent>(TEXT("CombatComponent"));
	//GetCapsuleComponent()->SetCapsuleHalfHeight();

	StateBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateBarUI"));
	StateBar->SetupAttachment(RootComponent);
	
	//StateBar->SetWidget(CreateWidget(UDXWidgetBase::StaticClass()));
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	StateBar->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()+7));
	
	UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
	ICSubSystem->MDG_ShowDebugChange.AddDynamic(this, &AEnemyCharacterBase::ShowDebugChange);

	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UCharacterAttributeSet::GetHPAttribute()).AddUObject(this, &AEnemyCharacterBase::OnHelthChange);
	}
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::OnHelthChange(const FOnAttributeChangeData& Data)
{
	if (ASC && !ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Died"))))
	{
		//回调蓝图处理UI
		HelthChangeBPDG.Broadcast(Data.NewValue);

		//死亡处理
		if (Data.NewValue <= 0.0f && CombatComp)
		{
			CombatComp->CharacterDied();
		}
	}
	
}

UAbilitySystemComponent* AEnemyCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AEnemyCharacterBase::ShowDebugChange(bool showDebug)
{
	GetCapsuleComponent()->SetVisibility(showDebug);
}

