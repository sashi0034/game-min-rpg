
local staticEvent = {
    "unit_1",
    "unit_2",
}


local mapEvent = {
    "house_1",
    "house_3",
    "house_2",
    "house_r_1",
    "house_r_3",
    "house_r_2",
    "house_on_hill",
    "tree_1",
    "tree_on_upstream",
    "herbs",
    "loophole_to_boss",
}


for index, value in ipairs(staticEvent) do
    loadfile([[.\asset\script\module\event\static_event\]]..value..[[.lua]])()
end

for index, value in ipairs(mapEvent) do
    loadfile([[.\asset\script\module\event\map_event\]]..value..[[.lua]])()
end




