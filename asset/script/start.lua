
loadfile([[./asset/script/config.lua]])()

loadfile([[./asset/script/globals.lua]])()

loadfile("./asset/script/load_script.lua")()

loadfile("./asset/script/module/ingame_event_base.lua")()

loadfile("./asset/script/module/flag_name.lua")()

loadfile([[./asset/script/module/player.lua]])()

loadfile([[.\asset\script\module\npc\load_npc.lua]])()

loadfile([[.\asset\script\module\map_event_manager.lua]])()
loadfile([[.\asset\script\module\event\load_events.lua]])()

loadfile([[./asset\script\module\message_window.lua]])()
loadfile([[./asset\script\module\selection_window.lua]])()

loadfile([[./asset\script\debug_start.lua]])()


-- @memo
-- event�֘A�͂����p�̃N���X��C++�ō����Lua�ŌĂԂ̂��悳����
-- ���W�Ȃǂ̓}�b�v�f�[�^�œǂݍ��񂾂��̂�C++�ŊǗ�����Lua�ŌĂ�

-- @memo
-- eventKind�ɂ�[touch, reach, unique]��3��ނ�����A
-- check��reach�͂��ꂼ��N���X�ŊǗ�����
-- �C�x���g�t���O��FlagMangaer: table�Ƃ����e�[�u���ŊǗ�

