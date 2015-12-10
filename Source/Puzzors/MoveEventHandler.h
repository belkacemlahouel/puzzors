// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UMovable;

/**
 * 
 */
class PUZZORS_API IMoveEventHandler
{
public: 
	virtual void OnMove(const UMovable* _sender) = 0;
};
