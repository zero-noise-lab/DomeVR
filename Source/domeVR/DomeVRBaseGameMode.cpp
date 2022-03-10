// Fill out your copyright notice in the Description page of Project Settings.


#include "DomeVRBaseGameMode.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red,text)

ADomeVRBaseGameMode::ADomeVRBaseGameMode()
{
	// Start building file name by getting actual date and time
	FDateTime Time = FDateTime();
	FDateTime CurrentTime = Time.Now();
	FileName = CurrentTime.ToString();
	const TCHAR * a = (const TCHAR*)&".";
	const TCHAR * b = (const TCHAR*)&"_";
	FileName = FileName.Replace(a,b);
	// Reserve memory for set buffer size
	ActLog.Reserve(BufferSize);
	SaveString = AbsoluteFilePath + FileName + "_" + WorldName;

}

/* Block with all definitions for retrieving different log types

To avoid multiple definitions of SendLog, LogData structs have to be converted
to classes. Then one SendLog method could be used that is overwritten by each
specific sublass. This is however not possible with structs since C++ is not 
supporting downcasting to overwritten methods here*/

void ADomeVRBaseGameMode::SendLog(FLogData SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
	
}

void ADomeVRBaseGameMode::SendTextLog(FLogDataText SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}

}

void ADomeVRBaseGameMode::SendIntLog(FLogDataInt SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendIntArrayLog(FLogDataIntArray SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendEventmarkerLog(FLogDataEventmarker SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel())
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendFloatLog(FLogDataFloat SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendFloatArrayLog(FLogDataFloatArray SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendVectorLog(FLogDataVector SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendVectorArrayLog(FLogDataVectorArray SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendGUIDLog(FLogDataGUID SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel()) 
	{
		ActLog.Add(SentLog.GetLogString());
	}
}

void ADomeVRBaseGameMode::SendUObjectIDsLog(FLogDataUE4ObjectArray SentLog)
{
	ActLogContinuous.Add(SentLog.GetLogString());

	if (VerbosityLevel >= SentLog.GetVerbosityLevel())
	{
		ActLog.Add(SentLog.GetLogString());
	}
}


void ADomeVRBaseGameMode::Tick(float DeltaSeconds)
{
	// Load viewport reference on first tick
	if (bLoadViewportReference)
	{
		bLoadViewportReference = false;
		if (GEngine)
		{ 
			FRHIViewport* const ViewportRHI = GEngine->GameViewport->Viewport->GetViewportRHI().GetReference();
			ViewportD3D11 = static_cast<FD3D11Viewport*>(ViewportRHI);

			if (!GEngine->GameViewport->Viewport->GetViewportRHI())
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No ViewportRHI"));

		}

	}

	// Generate log string for ViewportFrameStatistics based on current frame statistics from SwapChain
	FString SwapChainString;
	if (ViewportD3D11)
	{
		IDXGISwapChain* SwapChain;
		SwapChain = ViewportD3D11->GetSwapChain();
		DXGI_FRAME_STATISTICS Stats;
		if (SwapChain->GetFrameStatistics(&Stats) == S_OK)
		{
			uint32 ThisPresentId = 0;
			if (SwapChain->GetLastPresentCount(&ThisPresentId) == S_OK)
			{
				SwapChainString = SwapChainString.Printf(TEXT("%llu,%u,%u,%u,%u,%u,%u"),
					Stats.SyncQPCTime.QuadPart,
					Stats.PresentCount,
					ThisPresentId,
					Stats.PresentRefreshCount,
					Stats.SyncRefreshCount,
					GFrameCounter,
					GFrameNumber);
			}
		}
	}
	else 
	{
		if (Debug)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No valid reference to Viewport!"));
	}

	// Send ViewportFrameStatistics log to logging queue
	FLogDataText LogToSend;
	LogToSend.LogText = SwapChainString;
	LogToSend.Time = GetWorld()->GetRealTimeSeconds();
	LogToSend.LogType = LogTypes::ViewportFrameStatistics;
	LogToSend.OID = LogToSend.OID = LogToSend.OID.Printf(TEXT("%d"), this->GetUniqueID());
	LogToSend.VerbosityLevel = 5;
	SendTextLog(LogToSend);


	if (Debug) {
		FString BufferIndex;
		FString BufferIndexContinuous;
		BufferIndex.AppendInt(ActLog.Num());
		BufferIndexContinuous.AppendInt(ActLogContinuous.Num());
		print("ActLog Buffer");
		print(BufferIndex);
		print("ActLogContinuous Buffer");
		print(BufferIndexContinuous);
	}

	// Write lines to log file if log buffer is full and header is already written
	if (ActLog.Num() >= BufferSize && bHeaderWritten) 
	{
		WriteLog();
		ActLog.Reset(BufferSize);
	}

	if (ActLogContinuous.Num() >= BufferSize && bHeaderWritten)
	{
		WriteLogContinuous();
		ActLogContinuous.Reset(BufferSize);
	}
}


void ADomeVRBaseGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Get world name that is only accessible after game has started to generate file name
	WorldName = GetWorld()->GetName();
	SaveString = AbsoluteFilePath + FileName + "_" + WorldName; // +".log";

	// Create log header
	FDateTime Time = FDateTime().Now();
	HeaderString = WorldName + " started at " + Time.ToIso8601() + "\n";
	ActLog.Add("");
	ActLogContinuous.Add("");
}


void ADomeVRBaseGameMode::StartPlay()
{
	Super::StartPlay();

}

void ADomeVRBaseGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);
}

void ADomeVRBaseGameMode::WriteSessionDataHeader()
{
	FString Subject = "Subject: " + SessionData.Subject + "\n";
	FString Experiment = "Experiment: " + SessionData.Experiment + "\n";
	FString Session = "Session: " + SessionData.Session + "\n";
	FString Experimenter = "Experimenter: " + SessionData.Experimenter + "\n";
	FString SelectedLevel = "Selected Level: " + SessionData.SelectedLevel + "\n";
	FString SelectedStateMachine = "Selected State Machine: " + SessionData.SelectedStateMachine + "\n";
	FString SelectedResolution = "Set Resolution: " + SessionData.Resolution + "\n";
	FString ViewportResolution = "Actual Viewport Resolution: " + SessionData.ViewportResolution + "\n";
	FString RefreshRate = "Set Refresh Rate: " + SessionData.RefreshRate + "\n";
	FString VSyncState = "VSync: " + SessionData.VSyncState + "\n";
	FString SplitScreenOrder = "Splitscreen order: " + SessionData.SplitScreenOrder + "\n";
	FString RenderingMethod = "Rendering Method: " + SessionData.RenderingMethod + "\n";
	FString CaptureResolution = "Scene Capture Resolution: " + SessionData.CaptureResolution + "\n";
	FString FisheyeResolution = "Fisheye Resolution: " + SessionData.FisheyeResolution + "\n";
	FString InputForwardScale = "Input Forward Scale: " + SessionData.InputForwardScale + "\n";
	FString InputSidewardScale = "Input Sideward Scale: " + SessionData.InputSidewardScale + "\n";
	FString InputRotationScale = "Input Rotation Scale: " + SessionData.InputRotationScale + "\n";

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	FString QPFrequency = FString::Printf(TEXT("Query Performance Frequency : %llu"), freq.QuadPart);
 
	HeaderString.Append(Subject);
	HeaderString.Append(Experiment);
	HeaderString.Append(Session);
	HeaderString.Append(Experimenter);
	HeaderString.Append(SelectedLevel);
	HeaderString.Append(SelectedStateMachine);
	HeaderString.Append(SelectedResolution);
	HeaderString.Append(ViewportResolution);
	HeaderString.Append(RefreshRate);
	HeaderString.Append(VSyncState);
	HeaderString.Append(SplitScreenOrder);
	HeaderString.Append(RenderingMethod);
	HeaderString.Append(CaptureResolution);
	HeaderString.Append(FisheyeResolution);
	HeaderString.Append(InputForwardScale);
	HeaderString.Append(InputSidewardScale);
	HeaderString.Append(InputRotationScale);
	HeaderString.Append(QPFrequency);

	ActLog[0]			= HeaderString;
	ActLogContinuous[0] = HeaderString;
	bHeaderWritten		= true;
}


void ADomeVRBaseGameMode::UpdateSaveStringWithSessionData()
{
	FString RelativeFilePath;

	if (!SessionData.Experiment.IsEmpty())
	{
		RelativeFilePath = RelativeFilePath + "/" + SessionData.Experiment + "/";
		FileName = FileName + "_" + SessionData.Experiment;
	}

	if (!SessionData.Subject.IsEmpty())
	{
		RelativeFilePath = RelativeFilePath + "/" + SessionData.Subject + "/";
		FileName = FileName + "_" + SessionData.Subject;
	}

	FDateTime Time = FDateTime().Now();
	FString CurrentDate = FString::Printf(TEXT("%d%02d%02d"), Time.GetYear(), Time.GetMonth(), Time.GetDay());
	RelativeFilePath = RelativeFilePath + "/" + CurrentDate + "/";
	
	if (!SessionData.Experimenter.IsEmpty())
	{
		//RelativeFilePath = RelativeFilePath + "/" + SessionData.Experimenter + "/";
		FileName = FileName + "_" + SessionData.Experimenter;
	}

	if (!SessionData.Session.IsEmpty())
	{
		RelativeFilePath = RelativeFilePath + "/" + SessionData.Session + "/";
		FileName = FileName + "_" + SessionData.Session;
	}

	SaveString = AbsoluteFilePath + RelativeFilePath + FileName + "_" + WorldName;
}

void ADomeVRBaseGameMode::WriteLog()
{
	if (ActLog.Num() > 0)
	{
		FFileHelper::SaveStringArrayToFile(ActLog, *(SaveString + ".log"), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), FILEWRITE_Append);
	}
}

void ADomeVRBaseGameMode::WriteLogContinuous()
{
	if (ActLogContinuous.Num() > 0)
	{
		FFileHelper::SaveStringArrayToFile(ActLogContinuous, *(SaveString + "_Cont.log"), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), FILEWRITE_Append);
	}
}

void ADomeVRBaseGameMode::LoadLevel(FString LevelName, FString GameModePath) {
	FString PathOption = "?Game=";
	PathOption.Append(GameModePath);
	PathOption.Append("_C");
	APlayerController* UIController = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	UGameplayStatics::RemovePlayer(UIController, true);
	UGameplayStatics::OpenLevel(this, *LevelName, false, PathOption);
}

void ADomeVRBaseGameMode::BeginDestroy()
{
	WriteLog();
	WriteLogContinuous();
	Super::BeginDestroy();
}
