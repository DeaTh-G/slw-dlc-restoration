#pragma once

namespace slw_dlc_restoration
{
	class ObjYoshiCoin : public app::ObjYoshiCoin
	{
	public:
		void AddCallback(app::GameDocument* in_pDocument) override;
	};
}