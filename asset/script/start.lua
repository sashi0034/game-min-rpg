
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
-- event関連はそれ専用のクラスをC++で作ってLuaで呼ぶのがよさそう
-- 座標などはマップデータで読み込んだものをC++で管理してLuaで呼ぶ

-- @memo
-- eventKindには[touch, reach, unique]の3種類があり、
-- checkとreachはそれぞれクラスで管理する
-- イベントフラグはFlagMangaer: tableというテーブルで管理

