// Copyright DX_IC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectorActorBase.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectorHitObj, FHitResult, HitRes);

UENUM(BlueprintType)
enum class EHittedType : uint8
{
	AttachTo,
	Axplode
};

UCLASS()
class DX_INFINITECOMBAT_API AProjectorActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectorActorBase();

protected:
	// Called when the game starts or when spawned 
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//·¢Éä
	UFUNCTION(BlueprintCallable)
	virtual void Launch(float Intensity, FVector Velocity, float DestoryTime=.0f);
	//¿ªÆô/¹Ø±ÕÂ·¾¶ÃüÖÐ¼ì²â
	UFUNCTION(BlueprintCallable)
	void EnableTraceBySocketName(const TArray<TEnumAsByte<EObjectTypeQuery> >& TraceObjectTypes, bool Enable=false, const FString& TraceSocketPrefix = "DamageLoc");
	//×·×Ù
	UFUNCTION(BlueprintCallable)
	bool TraceObj(AActor* TraceObj);

protected:
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(BlueprintAssignable)
	FProjectorHitObj DG_ProjectorHitObj;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IC|Hitted")
	EHittedType HittedType = EHittedType::AttachTo;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "IC|Hitted")
	float HitDamage = 0.0f;

private:
	bool LineTraceByMeshSockets(const TArray<AActor*>& ActorsToIgnore, const TArray<TEnumAsByte<EObjectTypeQuery> >& TraceObjectTypes, bool bTraceComplex = false, bool bIgnoreSelf = true);
	
	FString TraceSocketPrefix = "DamageLoc";
	bool bEnableTrace = false;
	TMap<FName, FVector> PreSocketLoc;
	TArray<FName> AttackTraceSockets;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	bool bTrace = false;
	AActor* TraceActor;
};
