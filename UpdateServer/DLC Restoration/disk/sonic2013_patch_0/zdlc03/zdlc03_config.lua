---------------------------------------------------------------
-- @file	w3a03_config.lua
-- @author	Koizumi Machiko
-- @brief	w3-3 の設定ファイル
-- @data	2012/7/9
---------------------------------------------------------------

METER = 10.0

---------------------------------------------------------------

-- 落ち死にする高さ。これより下にプレイヤーが移動すると死にます。
dead_line = -150.0 * METER

-- カメラ設定.
camera = {
   near = 0.1 * METER,
   far = 25000.0 * METER,
   fov = 60,
}

-- サービスのリスト.
services = {

	-- グレア
   {
	  name = "CGlareManager",
	  params = {
		 ev = 1.0,
	  },
   },

}

-- 描画設定
gfx = {
	
	-- エフェクト設定
	fx = {
		-- カリング
		{
			name = "CullRange",
			params = {
				-- デフォルト
				range_default = 80000,
				
				-- 遠景
				range_far = 100000,
			},
		},
		
		-- ライト設定
		{
			name = "BasicLight",
			params = {
				-- グローバルライト(平行光)の有効・無効
				global_light_enable = true,
				
				-- アンビエントライトの有効・無効
				ambient_light_enable = false,
				
				-- ローカルライト(点光源）の有効・無効
				local_light_enable = false,
				
				-- カメラライトの有効・無効
				eye_light_enable = false,
				
				-- カメラライトモード(0:Directional 1:Omni)
				eye_light_mode = 1,
				
				-- カメラライト ディフューズ(RGB)
				eye_light_diffuse = { 0.19, 0.11, 0.07, },
				
				-- カメラライト スペキュラ(RGB)
				eye_light_specular = { 0.19, 0.11, 0.07, },
				
				-- カメラライト 距離減衰の開始距離
				eye_light_range_start = 800.0,
				
				-- カメラライト 距離減衰の終了距離
				eye_light_range_end = 2600.0,
			},
		},
		
		-- シーン描画設定
		{
			name = "Scene",
			params = {
				-- 天球の明るさ
				sky_intensity_scale = 1.43,
				
				-- 天球のY軸追従率
				sky_followup_ratio_y = 0.0,
			},
		},
		
		-- ライトフィールド設定
		{
			name = "LightField",
			params = {
				
				-- スケーリングタイプ(0:KeepingLuminance 1:KeepingBrightness 2:None)
				saturation_scaling_type = 1,
				
				-- データが存在しても無視する
				ignore_data = false,
				
				-- デフォルト更新頻度
				default_update_interval = 1,
				
				-- 終段の色補正をしない
				no_final_adjestcolor = true,
				
				-- インテンシティの閾値
				intensity_threshold = 0.5,
				
				-- 輝度最低値
				luminance_min = 0.0,
				
				-- 輝度最高値
				luminance_max = 1.0,
				
				-- 輝度中間値
				luminance_center = 0.5,
				
				-- 彩度スケール
				saturation_scaling_rate = 1.0,
				
				-- 輝度スケール
				luminance_scaling_rate = 1.0,
				
				-- デフォルト値_上から(RGB)
				default_color_up = {0.6, 0.72, 0.8, },
				
				-- デフォルト値_下から(RGB)
				default_color_down = {0.9, 1.02, 1.2, },
				
				-- オフセット値_上から(RGB)
				default_coloroffset_up = {0.0, 0.0, 0.0, },
				
				-- オフセット値_下から(RGB)
				default_coloroffset_down = {0.0, 0.0, 0.0, },
			},
		},
		
		-- ライトスキャッタリングなど
		{
			name = "LightScattering",
			params = {
				-- 共通
				-- 遠景と設定を分けるならtrue
				-- 分ける場合は、params_farも記述のこと.
				separate_enable = false,
				
				-- 有効・無効
				enable = true,
				
				-- カラー(RGB)
				color = { 0.04, 0.26, 0.80, },
				depth_scale = 0.3,
				
				--ライトスキャッタリング
				in_scattering_scale = 37.0,
				rayleigh = 3.00,
				mie = 3.60,
				mie_power = 0.46,
				near = 1650.0,
				far = 5300.0,
			},
			
			-- Farライトスキャッタリングなど(現在false)
			params_far = {
				-- 共通
				-- 有効・無効
				enable = true,
				
				-- カラー(RGB)
				color = { 0.05, 0.3, 0.6, },
				depth_scale = 0.05,
				
				--ライトスキャッタリング
				in_scattering_scale = 195.0,
				rayleigh = 8.50,
				mie = 5.00,
				mie_power = 1.00,
				near = 5000.0,
				far = 560000.0,
			
			},
		},
		
		-- HDR設定
		{
			name = "HDR",
			params = {
				-- トーンマップ有効・無効
				enable = true,
				
				-- タイプ(1:ControlExposure現在固定値)
				type = 1,
				
				-- ミドルグレー
				middle_gray = 0.54,
				
				-- トーンマップ調整項目
				luminance_low = 0.47,
				luminance_high = 0.78,
				-- 明暗順応有効?
				adaption = true,
				
			},
		},
		
		-- ブルーム＆スター設定
		{
			name = "BloomStar",
			params = {
				-- 有効・無効
				enable = true,
				
				-- グレアタイプ
				-- 0:Nothing 1:Natural 2:CheapLens 3:CrossScreen 4:SpectralCrossScreen 5:SnowCross
				-- 6:SpectralSnowCross 7:SunnyCross 8:SpectralSunnyCross 9:VerticalSlits 10:HorizontalSlits
				type = 1,
				
				brightpass_threshold = 0.96,
				brightpass_invscale = 6.20,
				persistent = 0.10,
				bloom_scale = 1.6,
				--↓使用していない（12/05/16）
				star_scale = 0.4,
			},
		},
		
		-- 被写界深度
		{
			name = "DepthofField",
			params = {
				-- 有効・無効
				enable = true,
				
				focus = 54.0,
				near = 14.0,
				far = 5000.0,
				focus_range = 32.0,
				
				-- 天球に被写界深度をかけない
				ignore_sky = false,
			},
		},
		
	}
}
