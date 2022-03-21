
local base = {
    "mouse",
    "punicat",
    "sheep",
    "slime",
    "chick",
    "chicken",
    "skull",
}


local named = {
    "cat_0",
    "cat_1",
    "cat_2",
    "cat_goto_hill",
    "mouse_fast",
    "sheep_1",
    "slime_on_bridge",
    "chicken_by_fence",
    "chick_1",
    "chick_2",
    "chick_3",
    "chick_4",
    "chick_5",
    "skull_last",
}


for index, value in ipairs(base) do
    loadfile([[.\asset\script\module\npc\]]..value..[[.lua]])()
end

for index, value in ipairs(named) do
    loadfile([[.\asset\script\module\npc\named\]]..value..[[.lua]])()
end




