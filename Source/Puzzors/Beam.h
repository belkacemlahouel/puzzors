// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Beam.generated.h"

class UReactOnLazorHit;
class ULazorManager;

#define BEAM_RED_COLOR FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)
#define BEAM_GREEN_COLOR FLinearColor(0.0f, 1.0f, 0.0f, 1.0f)
#define BEAM_BLUE_COLOR FLinearColor(0.0f, 0.0f, 1.0f, 1.0f)
#define BEAM_YELLOW_COLOR FLinearColor(1.0f, 1.0f, 0.0f, 1.0f)
#define BEAM_PURPLE_COLOR FLinearColor(1.0f, 0.0f, 1.0f, 1.0f)
#define BEAM_CYAN_COLOR FLinearColor(0.0f, 1.0f, 1.0f, 1.0f)
#define BEAM_WHITE_COLOR FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)
#define BEAM_NONE_COLOR FLinearColor(0.127f, 0.127f, 0.127f)
#define BEAM_DEFAULT_COLOR FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)

#define REGISTER_COLOR_BEGIN(var) switch(var) {
#define REGISTER_COLOR(a, b) case(EBeamColor::a): return b;
#define REGISTER_COLOR_END default: return FLinearColor(0.0f, 0.0f, 0.0f, 1.0f); }

#define REGISTER_ADD_COLOR(a, b, c) if((_color1 == EBeamColor::a && _color2 == EBeamColor::b) || (_color2 == EBeamColor::a && _color1 == EBeamColor::b)) return EBeamColor::c;

UENUM(BlueprintType)
enum class EBeamColor : uint8
{
	BC_RED		UMETA(DisplayName = "RedBeam"),
	BC_GREEN	UMETA(DisplayName = "GreenBeam"),
	BC_BLUE		UMETA(DisplayName = "BlueBeam"),
	BC_YELLOW	UMETA(DisplayName = "YellowBeam"),
	BC_PURPLE	UMETA(DisplayName = "PurpleBeam"),
	BC_CYAN		UMETA(DisplayName = "CyanBeam"),
	BC_WHITE	UMETA(DisplayName = "WhiteBeam"),

	BC_NONE		UMETA(DisplayName = "None"),
};

class FLazor
{
public:
	// ---- CONSTRUCTORS -----
	FLazor() { m_parent = NULL; m_particle = NULL; m_target = NULL; m_child = NULL; }
	FLazor(FLazor* _parent, UParticleSystemComponent* _particle, AActor* _target)
		: m_parent(_parent), m_particle(_particle), m_target(_target), m_child(NULL)
	{
	}

	~FLazor() { DestroyLazor(); }

	// ----- MUTATOR -----
	void SetParent(FLazor* _parent) { m_parent = _parent; }
	void SetChild(FLazor* _child) { m_child = _child; }

	void SetParticleSystem(UParticleSystemComponent* _particle) { m_particle = _particle; }
	void SetTarget(AActor* _target) { m_target = _target; }

	void SetPosition(FVector _position) { m_position = _position; }
	void SetDirection(FVector _direction) { m_direction = _direction; }

	// ----- ACCESSORS -----
	FLazor* Parent() const { return m_parent; }
	FLazor* Child() const { return m_child; }

	AActor* Target() { return m_target; }

	const UParticleSystemComponent* ParticleSystem() const { return m_particle; }
	UParticleSystemComponent* ParticleSystem() { return m_particle; }

	FVector Position() const { return m_position; }
	FVector Direction() const { return m_direction; }



	// ---- FUNCTIONS ----
	bool IsChild(FLazor* _lazor)
	{ 
		FLazor* l = Child();
		while (l != NULL)
		{
			if (l == _lazor)
				return true;
			l = l->Child();
		}
		return false;
	}

	// Count how many parent this lazor has
	int Index() const
	{
		int i = 0;
		const FLazor* l = Parent();
		while (l != NULL)
		{
			l = l->Parent();
			++i;
		}
		return i;
	}

	void DestroyLazor()
	{
		if (ParticleSystem() != NULL)
			ParticleSystem()->DestroyComponent();

		if (Target() != NULL)
			Target()->Destroy();

		if (Parent() != NULL)
			Parent()->SetChild(NULL);

		if (Child() != NULL)
			Child()->SetParent(NULL);

		SetChild(NULL);
		SetParent(NULL);
	}

private:
	// ---- ATTRIBUTES -----
	FLazor* m_child;
	FLazor* m_parent;
	UParticleSystemComponent* m_particle;
	AActor* m_target;

	FVector m_position;
	FVector m_direction;
};

USTRUCT()
struct FLazorHit
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		UReactOnLazorHit* reactor;
	
	FLazor* lazor;
	
	UPROPERTY(EditAnywhere)
		FHitResult hit;

	FLazor* Lazor() { return lazor; }
	UReactOnLazorHit* Reactor() { return reactor; }

	~FLazorHit() { reactor = NULL; lazor = NULL; }

	FLazorHit(UReactOnLazorHit* _reactor, FLazor* _lazor, FHitResult _hit) : reactor(_reactor), lazor(_lazor)
	{
		hit = _hit;
	}

	FLazorHit()
	{}
};

/**
 * 
 */
UCLASS(Blueprintable)
class PUZZORS_API ABeam : public AActor
{
	GENERATED_BODY()

public:
	ABeam();
	ABeam(const FVector& _direction);
	virtual ~ABeam();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "Beam")
		void Activate();
	
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void Deactivate();

	UFUNCTION(BlueprintCallable, Category = "Beam")
		void FireBeam();
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void ClearBeam(bool _callHitEnd);

	bool HasHit(UReactOnLazorHit* _reactor);

	bool Initialized() { return ParticleTemplate != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Beam")
		static FLinearColor BeamColorToLinearColor(EBeamColor _color);

	UFUNCTION(BlueprintCallable, Category = "Beam")
		static EBeamColor AddColor(EBeamColor _color1, EBeamColor _color2);

public:
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void SetDirection(const FVector& _direction) { m_direction = _direction; }
	
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void SetColor(EBeamColor _color) { m_color = _color; }

	UFUNCTION(BlueprintCallable, Category = "Beam")
		EBeamColor Color() { return m_color; }

	UFUNCTION(BlueprintCallable, Category = "Beam")
		bool IsFired() { return m_fired; }

private:
	FLazor* m_root;
	FVector m_direction;

	EBeamColor m_color;

	TArray<FLazorHit> m_hit;

	FTimerHandle m_TimerHandle;

	bool m_fired;

private:
	FLazor* InstanciateLazor(UParticleSystemComponent* _particle, FLazor* _parent)
	{
		FLazor* lazor = new FLazor();
		lazor->SetParticleSystem(_particle);
		lazor->SetParent(_parent);
		if (_parent != NULL) _parent->SetChild(lazor);
		return lazor;
	}
	UParticleSystemComponent* InstantiateParticleSystem(const FVector& _location, const FRotator& _direction);

	AActor* ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult);
	UReactOnLazorHit* ExtractReactorFromActor(AActor* _actor);

	UParticleSystem* ParticleTemplate;
};
