

local files = {
    "house_1",
    "house_3",
    "tree_1",
    "tree_on_upstream",
}


for index, value in ipairs(files) do
    loadfile([[.\asset\scripte\module\event\content\]]..value..[[.lua]])()
end




