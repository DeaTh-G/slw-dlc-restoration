---------------------------------------------------------------
-- @file	w1a03_config.lua
-- @author	Miura Yoshitaka
-- @brief	w1-3 の設定ファイル
-- @data	2012/5/11
---------------------------------------------------------------

METER = 10.0

---------------------------------------------------------------

-- 落ち死にする高さ。これより下にプレイヤーが移動すると死にます。
dead_line = -500.0 * METER

-- カメラ設定.
camera = {
   near = 0.1 * METER,
   far = 10000.0 * METER,
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
	-- エッグ
   {
	  name = "EggManager",
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
				eye_light_diffuse = { 1.0, 1.0, 1.0, },
				
				-- カメラライト スペキュラ(RGB)
				eye_light_specular = { 1.0, 1.0, 1.0, },
				
				-- カメラライト 距離減衰の開始距離
				eye_light_range_start = 0.0,
				
				-- カメラライト 距離減衰の終了距離
				eye_light_range_end = 100.0,
			},
		},
		
		-- シーン描画設定
		{
			name = "Scene",
			params = {
				-- 天球の明るさ
				sky_intensity_scale = 1.0,
				
				-- 天球のY軸追従率
				sky_followup_ratio_y = 1.0,
			},
		},
		
		-- ライトフィールド設定
		{
			name = "LightField",
			params = {
				
				-- スケーリングタイプ(0:KeepingLuminance 1:KeepingBrightness 2:None)
				saturation_scaling_type = 0,
				
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
				default_color_up = {0.85, 0.85, 0.85, },
				
				-- デフォルト値_下から(RGB)
				default_color_down = {1.1, 1.1, 1.1, },
				
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
				color = { 0.0, 0.0, 0.0, },
				depth_scale = 0.07,
				
				--ライトスキャッタリング
				in_scattering_scale = 300.0,
				rayleigh = 4.0,
				mie = 7.00,
				mie_power = 1.00,
				near = 50.0,
				far = 1500.0,
			},
			
			-- Farライトスキャッタリングなど
			params_far = {
				-- 共通
				-- 有効・無効
				enable = true,
				
				-- カラー(RGB)
				color = { 0.03, 0.13, 0.45, },
				depth_scale = 0.10,
				
				--ライトスキャッタリング
				in_scattering_scale = 270.0,
				rayleigh = 6.50,
				mie = 5.00,
				mie_power = 1.00,
				near = 1400.0,
				far = 460000.0,
			
			},
		},
		
		-- HDR設定
		{
			name = "HDR",
			params = {
				-- 有効・無効
				enable = true,
				
				-- タイプ(0:SimpleScale 1:ControlExposure 2:OriginalToneMap 3:CustomToneMap)
				type = 1,
				
				-- 明暗順応有効?
				adaption = true,
				
				middle_gray = 0.50,
				scale = 0.5,
				luminance_low = 0.38,
				luminance_high = 0.57,
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
				
				brightpass_threshold = 0.99,
				brightpass_invscale = 8.26,
				persistent = 0.175,
				bloom_scale = 0.8,
				
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

		-- 時勢変化
		{
			name = "TimesMidifier",
			params = {
				-- 有効・無効
				use_times_modifier = true,
				
				--天球はそのままの色にするか.
				ignore_sky_modifier = true,
				
				-- 各時勢の時刻。0から23.99まで
				
				time_morning = 7,
				time_day = 13,
				time_evening = 17,
				time_night = 23,
			},
			
			--朝
			params_morning = {
				-- HDR
				middle_gray = 1,
				
				-- 基本色
				basecol_r = 1,	
				basecol_g = 1,	
				basecol_b = 1,	
				
				-- 並行光源
				light_r = 1,	
				light_g = 1,	
				light_b = 1,	
				
				-- 天球
				sky_intensity = 1,
				
				sky_r = 1,
				sky_g = 1,
				sky_b = 1,
				
				-- アンビエント
				ambient_r = 1,
				ambient_g = 1,
				ambient_b = 1,
				
				-- ライトスキャッタリング
				light_scattering_r = 1,
				light_scattering_g = 1,
				light_scattering_b = 1,
			},
			

			--昼
			params_day = {
				-- HDR
				middle_gray = 1,
				
				-- 基本色
				basecol_r = 1,	
				basecol_g = 1,	
				basecol_b = 1,	
				
				-- 並行光源
				light_r = 1,	
				light_g = 1,	
				light_b = 1,	
				
				-- 天球
				sky_intensity = 1,
				
				sky_r = 1,
				sky_g = 1,
				sky_b = 1,
				
				-- アンビエント
				ambient_r = 1,
				ambient_g = 1,
				ambient_b = 1,
				
				-- ライトスキャッタリング
				light_scattering_r = 1,
				light_scattering_g = 1,
				light_scattering_b = 1,
			},
				

			--夕方
			params_evening = {
				-- HDR
				middle_gray = 1.0,
				
				-- 基本色
				basecol_r = 1.05,	
				basecol_g = 0.85,	
				basecol_b = 0.45,	
				
				-- 並行光源
				light_r = 1.2,	
				light_g = 1.1,	
				light_b = 1.0,	
				
				-- 天球
				sky_intensity = 1.15,
				
				sky_r = 1.45,
				sky_g = 0.8,
				sky_b = 0.5,
				
				-- アンビエント
				ambient_r = 1.1,
				ambient_g = 0.9,
				ambient_b = 0.7,
				
				-- ライトスキャッタリング
				light_scattering_r = 1.0,
				light_scattering_g = 1.0,
				light_scattering_b = 1.0,
			},
												

			--夜
			params_night = {
				-- HDR
				middle_gray = 1.0,
				
				-- 基本色
				basecol_r = 0.5,	
				basecol_g = 0.9,	
				basecol_b = 1.2,	
				
				-- 並行光源
				light_r = 0.2,	
				light_g = 0.4,	
				light_b = 0.7,	
				
				-- 天球
				sky_intensity = 1.0,
				
				sky_r = 0.3,
				sky_g = 0.3,
				sky_b = 0.65,
				
				-- アンビエント
				ambient_r = 0.9,
				ambient_g = 1.0,
				ambient_b = 1.2,
				
				-- ライトスキャッタリング
				light_scattering_r = 1.0,
				light_scattering_g = 1.0,
				light_scattering_b = 1.0,
			},											
		},		
	}
}
