#include "pch.h"
#include "ObjYoshiCoin.h"

void slw_dlc_restoration::ObjYoshiCoin::AddCallback(app::GameDocument* in_pDocument)
{
	app::fnd::GOComponent::Create<app::game::GOCShadowSimple>(*this);
	app::ObjYoshiCoin::AddCallback(in_pDocument);
}