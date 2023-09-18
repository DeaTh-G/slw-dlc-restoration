---------------------------------------------------------------
-- @file	w3a03_config.lua
-- @author	Koizumi Machiko
-- @brief	w3-3 �̐ݒ�t�@�C��
-- @data	2012/7/9
---------------------------------------------------------------

METER = 10.0

---------------------------------------------------------------

-- �������ɂ��鍂���B�����艺�Ƀv���C���[���ړ�����Ǝ��ɂ܂��B
dead_line = -150.0 * METER

-- �J�����ݒ�.
camera = {
   near = 0.1 * METER,
   far = 25000.0 * METER,
   fov = 60,
}

-- �T�[�r�X�̃��X�g.
services = {

	-- �O���A
   {
	  name = "CGlareManager",
	  params = {
		 ev = 1.0,
	  },
   },

}

-- �`��ݒ�
gfx = {
	
	-- �G�t�F�N�g�ݒ�
	fx = {
		-- �J�����O
		{
			name = "CullRange",
			params = {
				-- �f�t�H���g
				range_default = 80000,
				
				-- ���i
				range_far = 100000,
			},
		},
		
		-- ���C�g�ݒ�
		{
			name = "BasicLight",
			params = {
				-- �O���[�o�����C�g(���s��)�̗L���E����
				global_light_enable = true,
				
				-- �A���r�G���g���C�g�̗L���E����
				ambient_light_enable = false,
				
				-- ���[�J�����C�g(�_�����j�̗L���E����
				local_light_enable = false,
				
				-- �J�������C�g�̗L���E����
				eye_light_enable = false,
				
				-- �J�������C�g���[�h(0:Directional 1:Omni)
				eye_light_mode = 1,
				
				-- �J�������C�g �f�B�t���[�Y(RGB)
				eye_light_diffuse = { 0.19, 0.11, 0.07, },
				
				-- �J�������C�g �X�y�L����(RGB)
				eye_light_specular = { 0.19, 0.11, 0.07, },
				
				-- �J�������C�g ���������̊J�n����
				eye_light_range_start = 800.0,
				
				-- �J�������C�g ���������̏I������
				eye_light_range_end = 2600.0,
			},
		},
		
		-- �V�[���`��ݒ�
		{
			name = "Scene",
			params = {
				-- �V���̖��邳
				sky_intensity_scale = 1.43,
				
				-- �V����Y���Ǐ]��
				sky_followup_ratio_y = 0.0,
			},
		},
		
		-- ���C�g�t�B�[���h�ݒ�
		{
			name = "LightField",
			params = {
				
				-- �X�P�[�����O�^�C�v(0:KeepingLuminance 1:KeepingBrightness 2:None)
				saturation_scaling_type = 1,
				
				-- �f�[�^�����݂��Ă���������
				ignore_data = false,
				
				-- �f�t�H���g�X�V�p�x
				default_update_interval = 1,
				
				-- �I�i�̐F�␳�����Ȃ�
				no_final_adjestcolor = true,
				
				-- �C���e���V�e�B��臒l
				intensity_threshold = 0.5,
				
				-- �P�x�Œ�l
				luminance_min = 0.0,
				
				-- �P�x�ō��l
				luminance_max = 1.0,
				
				-- �P�x���Ԓl
				luminance_center = 0.5,
				
				-- �ʓx�X�P�[��
				saturation_scaling_rate = 1.0,
				
				-- �P�x�X�P�[��
				luminance_scaling_rate = 1.0,
				
				-- �f�t�H���g�l_�ォ��(RGB)
				default_color_up = {0.6, 0.72, 0.8, },
				
				-- �f�t�H���g�l_������(RGB)
				default_color_down = {0.9, 1.02, 1.2, },
				
				-- �I�t�Z�b�g�l_�ォ��(RGB)
				default_coloroffset_up = {0.0, 0.0, 0.0, },
				
				-- �I�t�Z�b�g�l_������(RGB)
				default_coloroffset_down = {0.0, 0.0, 0.0, },
			},
		},
		
		-- ���C�g�X�L���b�^�����O�Ȃ�
		{
			name = "LightScattering",
			params = {
				-- ����
				-- ���i�Ɛݒ�𕪂���Ȃ�true
				-- ������ꍇ�́Aparams_far���L�q�̂���.
				separate_enable = false,
				
				-- �L���E����
				enable = true,
				
				-- �J���[(RGB)
				color = { 0.04, 0.26, 0.80, },
				depth_scale = 0.3,
				
				--���C�g�X�L���b�^�����O
				in_scattering_scale = 37.0,
				rayleigh = 3.00,
				mie = 3.60,
				mie_power = 0.46,
				near = 1650.0,
				far = 5300.0,
			},
			
			-- Far���C�g�X�L���b�^�����O�Ȃ�(����false)
			params_far = {
				-- ����
				-- �L���E����
				enable = true,
				
				-- �J���[(RGB)
				color = { 0.05, 0.3, 0.6, },
				depth_scale = 0.05,
				
				--���C�g�X�L���b�^�����O
				in_scattering_scale = 195.0,
				rayleigh = 8.50,
				mie = 5.00,
				mie_power = 1.00,
				near = 5000.0,
				far = 560000.0,
			
			},
		},
		
		-- HDR�ݒ�
		{
			name = "HDR",
			params = {
				-- �g�[���}�b�v�L���E����
				enable = true,
				
				-- �^�C�v(1:ControlExposure���݌Œ�l)
				type = 1,
				
				-- �~�h���O���[
				middle_gray = 0.54,
				
				-- �g�[���}�b�v��������
				luminance_low = 0.47,
				luminance_high = 0.78,
				-- ���Ï����L��?
				adaption = true,
				
			},
		},
		
		-- �u���[�����X�^�[�ݒ�
		{
			name = "BloomStar",
			params = {
				-- �L���E����
				enable = true,
				
				-- �O���A�^�C�v
				-- 0:Nothing 1:Natural 2:CheapLens 3:CrossScreen 4:SpectralCrossScreen 5:SnowCross
				-- 6:SpectralSnowCross 7:SunnyCross 8:SpectralSunnyCross 9:VerticalSlits 10:HorizontalSlits
				type = 1,
				
				brightpass_threshold = 0.96,
				brightpass_invscale = 6.20,
				persistent = 0.10,
				bloom_scale = 1.6,
				--���g�p���Ă��Ȃ��i12/05/16�j
				star_scale = 0.4,
			},
		},
		
		-- ��ʊE�[�x
		{
			name = "DepthofField",
			params = {
				-- �L���E����
				enable = true,
				
				focus = 54.0,
				near = 14.0,
				far = 5000.0,
				focus_range = 32.0,
				
				-- �V���ɔ�ʊE�[�x�������Ȃ�
				ignore_sky = false,
			},
		},
		
	}
}
