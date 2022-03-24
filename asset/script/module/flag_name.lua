
FlagName = {
    broke_tree_branch=[[木の枝を折った]],
    have_tree_branch=[[木の枝を手に入れた]],
    have_wood_sword=[[木刀を手に入れた]],
    kill_bee=[[蜂を倒した]],
    obtain_honey=[[はちみつを手に入れた]],
    heard_mouse_rumor=[[ネズミのうわさを聞いた]],
    obtain_super_dash=[[速く走れるようになった]],
    friend_with_mouse=[[ネズミと友達になった]],
    obtain_solt=[[岩塩を手に入れた]],
    kill_slime=[[スライムを倒した]],
    pop_chick_1st = [[迷子のひよこの話を聞いた]],
    pop_chick_2nd = [[ヒヨコはまだいそうだ]],
    catch_chick_1 = [[ヒヨコAを保護した]],
    catch_chick_2 = [[ヒヨコBを保護した]],
    catch_chick_3 = [[ヒヨコCを保護した]],
    catch_chick_4 = [[ヒヨコDを保護した]],
    catch_chick_5 = [[ヒヨコEを保護した]],
    send_all_chick = [[ヒヨコたちを送り届けた]],
    obtain_herbs_near_bush = [[薬草Cを手に入れた]],
    obtain_herbs_under_tree = [[薬草Bを手に入れた]],
    obtain_herbs_near_castle = [[薬草Aを手に入れた]],
    gave_herbs = [[薬草をあげた]],
    try_dump_paper = [[書類を処分する依頼を受けた]],
    dump_paper_by_eat = [[書類を処分した]],
    talked_with_cat_goto_hill = [[走ってたネコと話した]],
    find_loophole_to_boss = [[隠された抜け道を見つけた]],
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



