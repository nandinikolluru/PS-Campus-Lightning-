// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketTestGameInstance.h"
#include "WebSocketsModule.h"
#include "Components/SceneComponent.h"
#include "Components/PointLightComponent.h"

void UWebSocketTestGameInstance::Init()
{
	Super::Init();
	//FWebSocketsModule& Module = FModuleManager::LoadModuleChecked<FWebSocketsModule>(TEXT("WebSockets"));
    //Socket = FWebSocketsModule::Get().CreateWebSocket("ws://localhost:8080");
    if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
    {
        FModuleManager::Get().LoadModule("WebSockets");
    }

    Socket = FWebSocketsModule::Get().CreateWebSocket("wss://2t6ne6hjea.execute-api.us-east-1.amazonaws.com/production");

    Socket->OnConnected().AddLambda([]()
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Successfully connected");
        });

    Socket->OnConnectionError().AddLambda([](const FString& Error) 
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, Error);
        });

    Socket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) 
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, bWasClean ? FColor::Green : FColor::Red, "Connection closed " + Reason);
        // This code will run when the connection to the server has been terminated.
        // Because of an error or a call to Socket->Close().
        });

    Socket->OnMessage().AddLambda([](const FString& Message) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "Received message: " + Message);
        // This code will run when we receive a string message from the server.
        });

    Socket->OnMessageSent().AddLambda([](const FString& MessageString) 
        {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Sent message: " + MessageString);
        // This code is called after we sent a message to the server.
        });

    // And we finally connect to the server. 
    Socket->Connect();
}

void UWebSocketTestGameInstance::Shutdown()
{
    if (Socket->IsConnected())
    {
        Socket->Close();
    }
    Super::Shutdown();
}

