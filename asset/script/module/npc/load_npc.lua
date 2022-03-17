
local base = {
    "mouse",
    "punicat",
    "sheep",
    "slime",
}


local named = {
    "cat_0",
    "cat_1",
    "cat_2",
    "cat_goto_hill",
    "mouse_fast",
    "sheep_1",
    "slime_on_bridge"
}


for index, value in ipairs(base) do
    loadfile([[.\asset\script\module\npc\]]..value..[[.lua]])()
end

for index, value in ipairs(named) do
    loadfile([[.\asset\script\module\npc\named\]]..value..[[.lua]])()
end




