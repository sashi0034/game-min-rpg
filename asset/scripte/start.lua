
loadfile([[./asset/scripte/config.lua]])()

loadfile([[./asset/scripte/globals.lua]])()

loadfile("./asset/scripte/load_scripte.lua")()

loadfile("./asset/scripte/module/test.lua")()

loadfile("./asset/scripte/module/ingame_event_base.lua")()

loadfile("./asset/scripte/module/flag_name.lua")()

-- Player
loadfile([[./asset/scripte/module/player.lua]])()

-- NPC
loadfile([[.\asset\scripte\module\npc\punicat.lua]])()
loadfile([[.\asset\scripte\module\npc\cat_1.lua]])()
loadfile([[.\asset\scripte\module\npc\cat_2.lua]])()
loadfile([[.\asset\scripte\module\npc\slime.lua]])()
loadfile([[.\asset\scripte\module\npc\slime_on_bridge.lua]])()

loadfile([[.\asset\scripte\module\map_event_manager.lua]])()
loadfile([[.\asset\scripte\module\map_events.lua]])()

loadfile([[./asset\scripte\module\message_window.lua]])()
loadfile([[./asset\scripte\module\selection_window.lua]])()

loadfile([[./asset/scripte/module/test_npc.lua]])()

-- @memo
-- event�֘A�͂����p�̃N���X��C++�ō����Lua�ŌĂԂ̂��悳����
-- ���W�Ȃǂ̓}�b�v�f�[�^�œǂݍ��񂾂��̂�C++�ŊǗ�����Lua�ŌĂ�

-- @memo
-- eventKind�ɂ�[touch, reach, unique]��3��ނ�����A
-- check��reach�͂��ꂼ��N���X�ŊǗ�����
-- �C�x���g�t���O��FlagData: table�Ƃ����e�[�u���ŊǗ�

