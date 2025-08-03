// Copyright DX_IC


#include "ProjectorActorBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DX_StaticFunlib.h"
#include "KismetTraceUtils.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Damage.h"
#include "ICWorldSubsystem.h"

// Sets default values
AProjectorActorBase::AProjectorActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
    StaticMeshComp->SetupAttachment(RootComponent);
    StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectorMoveComp"));
    //ProjectileMovementComponent->SetUpdatedComponent(StaticMeshComp);
    ProjectileMovementComponent->InitialSpeed = 1200;
    ProjectileMovementComponent->MaxSpeed = 0.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = false;
    /*ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;*/
    ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

}

// Called when the game starts or when spawned
void AProjectorActorBase::BeginPlay()
{
	Super::BeginPlay();
	
    ProjectileMovementComponent->bSimulationEnabled = false;

}

// Called every frame
void AProjectorActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bEnableTrace)
    {
        LineTraceByMeshSockets(TArray<AActor*>({GetOwner()}), TraceObjectTypes);
    }
    if (bTrace)
    {
        FVector Velocity = (TraceActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
        ProjectileMovementComponent->Velocity = Velocity * ProjectileMovementComponent->InitialSpeed;
    }

}

void AProjectorActorBase::Launch(float Intensity, FVector Velocity, float DestoryTime)
{
    ProjectileMovementComponent->Velocity = Velocity * ProjectileMovementComponent->InitialSpeed;
    ProjectileMovementComponent->bSimulationEnabled = true;
    SetLifeSpan(DestoryTime);
}

void AProjectorActorBase::EnableTraceBySocketName(const TArray<TEnumAsByte<EObjectTypeQuery> >& TraceObjectTypesarg, bool Enable, const FString& TraceSocketPrefixarg)
{
    bEnableTrace = Enable;
    if (Enable)
    {
        this->TraceSocketPrefix = TraceSocketPrefixarg;
        AttackTraceSockets.Empty();
        PreSocketLoc.Empty();

        for (auto& socket : StaticMeshComp->GetAllSocketNames())
        {
            if (socket.ToString().Contains(TraceSocketPrefix))
            {
                AttackTraceSockets.AddUnique(socket);
                PreSocketLoc.Add(socket, StaticMeshComp->GetSocketLocation(socket));
            }
        }
        this->TraceObjectTypes = TraceObjectTypesarg;
    }
       
}

bool AProjectorActorBase::TraceObj(AActor* TraceObj)
{
    if (TraceObj)
    {
        bTrace = true;
        TraceActor = TraceObj;
        return true;
    }
    return false;

}

bool AProjectorActorBase::LineTraceByMeshSockets(const TArray<AActor*>& ActorsToIgnore, const TArray<TEnumAsByte<EObjectTypeQuery> >& TraceObjectTypesarg, bool bTraceComplex, bool bIgnoreSelf)
{
    static  FCollisionQueryParams CollisionQueryParams;
    static FCollisionObjectQueryParams ObjectQueryParams;
    static FHitResult OutHit;

    UDX_StaticFunlib::MakeCollisionParam(this, CollisionQueryParams, ObjectQueryParams, ActorsToIgnore, TraceObjectTypesarg, bTraceComplex, bIgnoreSelf);

    for (auto& socket : AttackTraceSockets)
    {
        bool const bHit = GetWorld()->LineTraceSingleByObjectType(OutHit, *PreSocketLoc.Find(socket), StaticMeshComp->GetSocketLocation(socket), ObjectQueryParams, CollisionQueryParams);

        UICWorldSubsystem* ICSubSystem = UWorld::GetSubsystem<UICWorldSubsystem>(GetWorld());
        if (ICSubSystem->GetShowDebug())
            DrawDebugLineTraceSingle(GetWorld(), *PreSocketLoc.Find(socket), StaticMeshComp->GetSocketLocation(socket), EDrawDebugTrace::Type::ForDuration, bHit, OutHit, FColor::Red, FColor::Green, 1.5f);

        PreSocketLoc.Add(socket, StaticMeshComp->GetSocketLocation(socket));
        if (bHit)
        {
            //命中处理
            if(HittedType == EHittedType::AttachTo)
                AttachToActor(OutHit.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
            else if (HittedType == EHittedType::Axplode)
            {
                SetLifeSpan(0.4f);
            }

            //停止发射物
            bEnableTrace = false;
            ProjectileMovementComponent->StopMovementImmediately();
            ProjectileMovementComponent->bSimulationEnabled = false;

            //伤害感知事件发送
            UAISense_Damage::ReportDamageEvent(GetOwner()->GetWorld(), OutHit.GetActor(), GetOwner(), 0.f, GetOwner()->GetActorLocation(), OutHit.GetActor()->GetActorLocation());
            
            //冲力
            UPrimitiveComponent* Primi = OutHit.GetActor()->FindComponentByClass<UPrimitiveComponent>();
            if(Primi->IsSimulatingPhysics())
                Primi->AddImpulse(OutHit.ImpactNormal*-1*700, NAME_None, true);

            //广播到蓝图
            DG_ProjectorHitObj.Broadcast(OutHit);

            return true;
        }
    }

    return false;
}

