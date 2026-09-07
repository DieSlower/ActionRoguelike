// Fill out your copyright notice in the Description page of Project Settings.


#include "URogueDeveloperSettings.h"

FName URogueDeveloperSettings::GetCategoryName() const
{
	return FApp::GetProjectName();
}
