// Copyright DX_IC


#include "Character/EnemyCharacterBase.h"
#include "GAS/ICAbilitySystemComponent.h"
#include "UMG/DXWidgetBase.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "ICComponents/CombatCharacterComponent.h"
#include "GAS/CharacterAttributeSet.h"
#include "Subsystem/ICWorldSubsystem.h"

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

UAbilitySystemComponent* AEnemyCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AEnemyCharacterBase::ShowDebugChange(bool showDebug)
{
	GetCapsuleComponent()->SetVisibility(showDebug);
}

