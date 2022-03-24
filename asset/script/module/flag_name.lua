
FlagName = {
    broke_tree_branch=[[�؂̎}��܂���]],
    have_tree_branch=[[�؂̎}����ɓ��ꂽ]],
    have_wood_sword=[[�ؓ�����ɓ��ꂽ]],
    kill_bee=[[�I��|����]],
    obtain_honey=[[�͂��݂���ɓ��ꂽ]],
    heard_mouse_rumor=[[�l�Y�~�̂��킳�𕷂���]],
    obtain_super_dash=[[���������悤�ɂȂ���]],
    friend_with_mouse=[[�l�Y�~�ƗF�B�ɂȂ���]],
    obtain_solt=[[�≖����ɓ��ꂽ]],
    kill_slime=[[�X���C����|����]],
    pop_chick_1st = [[���q�̂Ђ悱�̘b�𕷂���]],
    pop_chick_2nd = [[�q���R�͂܂���������]],
    catch_chick_1 = [[�q���RA��ی삵��]],
    catch_chick_2 = [[�q���RB��ی삵��]],
    catch_chick_3 = [[�q���RC��ی삵��]],
    catch_chick_4 = [[�q���RD��ی삵��]],
    catch_chick_5 = [[�q���RE��ی삵��]],
    send_all_chick = [[�q���R�����𑗂�͂���]],
    obtain_herbs_near_bush = [[��C����ɓ��ꂽ]],
    obtain_herbs_under_tree = [[��B����ɓ��ꂽ]],
    obtain_herbs_near_castle = [[��A����ɓ��ꂽ]],
    gave_herbs = [[�򑐂�������]],
    try_dump_paper = [[���ނ���������˗����󂯂�]],
    dump_paper_by_eat = [[���ނ���������]],
    talked_with_cat_goto_hill = [[�����Ă��l�R�Ƙb����]],
    find_loophole_to_boss = [[�B���ꂽ��������������]],
}

FlagDescriptions = {}

for key, value in pairs(FlagName) do
    FlagDescriptions[key] = value
    FlagName[key] = key..""
end

CountFlagForPray = function ()
    local count = 0
    local prayFlagInc = function (flagname)
        if FlagManager.getFlag(flagname) then
            count = count+1
        end
    end
    prayFlagInc(FlagName.obtain_solt)
    prayFlagInc(FlagName.kill_slime)
    prayFlagInc(FlagName.talked_with_cat_goto_hill)
    prayFlagInc(FlagName.friend_with_mouse)
    prayFlagInc(FlagName.gave_herbs)
    prayFlagInc(FlagName.send_all_chick)
    prayFlagInc(FlagName.dump_paper_by_eat)
    return count
end



