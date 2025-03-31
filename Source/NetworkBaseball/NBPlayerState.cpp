#include "NBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "NBGameMode.h"
#include "NBGameProgressWidget.h"
#include "NBHUDWidget.h"
#include "NBPlayerController.h"
#include "NBSBOScreenWidget.h"
#include "NetworkBaseball.h"

ANBPlayerState::ANBPlayerState()
{
	InningCount = 1;
	OutCount = 0;
	StrikeCount = 0;
	BallCount = 0;

	HitCount = 0;
	GameScore = 0;

	UserPitchString = TEXT("");
	ComputerPitchString = TEXT("");
	
	bOnlyRelevantToOwner = true;
	UserName = TEXT("");
}

void ANBPlayerState::SetUserPitchString(const FString& NewValue)
{
	UserPitchString = NewValue;

	if (HasAuthority())
	{
		RequestGenerateComputerPitch();
		RequestJudgePlayResult();
		RequestUpdateScreen();
	}
}

void ANBPlayerState::ResetUserPitchString()
{
	if (HasAuthority())
	{
		UserPitchString = TEXT("");
		RequestUpdateProgress();
	}
}

void ANBPlayerState::RequestGenerateComputerPitch()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	auto* GameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		FString NewPitch = GameMode->GenerateComputerNumber();
		SetComputerPitchString(NewPitch);
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
}

void ANBPlayerState::RequestUpdateScreen()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}

	ANBPlayerController* NBPlayerController = Cast<ANBPlayerController>(PlayerController);
	if (NBPlayerController)
	{
		auto* ScreenWidget = Cast<UNBSBOScreenWidget>(NBPlayerController->GetHUDWidgetInstance()->SBOScreenWidget);
		if (ScreenWidget)
		{
			ScreenWidget->UpdateScreen();
		}
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerState::RequestUpdateProgress()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	auto* PlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		auto* ProgressWidget = Cast<UNBGameProgressWidget>(PlayerController->GetHUDWidgetInstance()->ProgressWidget);
		if (ProgressWidget)
		{
			ProgressWidget->UpdateDisplayInfo();
		}
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerState::RequestJudgePlayResult()
{
	auto* PlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->ServerRPCRequestJudgePlay(UserName);
	}
}


void ANBPlayerState::OnRep_UserName()            
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_InningCount()         
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_OutCount()            
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_StrikeCount()         
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_BallCount()           
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_HitCount()            
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_GameScore()           
{ 
    RequestUpdateScreen(); 
}
void ANBPlayerState::OnRep_UserPitchString()     
{
    RequestUpdateScreen();
	RequestJudgePlayResult();
	
}
void ANBPlayerState::OnRep_ComputerPitchString() 
{ 
    RequestUpdateScreen();
	RequestUpdateProgress();
}

void ANBPlayerState::OnRep_JudgePlayResult()
{
	RequestUpdateScreen();
}


void ANBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBPlayerState, UserName);
	DOREPLIFETIME(ANBPlayerState, InningCount);
	DOREPLIFETIME(ANBPlayerState, OutCount);
	DOREPLIFETIME(ANBPlayerState, StrikeCount);
	DOREPLIFETIME(ANBPlayerState, BallCount);
	DOREPLIFETIME(ANBPlayerState, HitCount);
	DOREPLIFETIME(ANBPlayerState, GameScore);
	DOREPLIFETIME(ANBPlayerState, UserPitchString);
	DOREPLIFETIME(ANBPlayerState, ComputerPitchString);
	DOREPLIFETIME(ANBPlayerState, PlayResult);
}
