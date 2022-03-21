MapEventManager:addMapEvents({

    house_1 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[何か用かね?]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[情報交換]], [[いいえ]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            if not FlagManager.getFlag(FlagName.broke_tree_branch) then
                m:streamText("\n"..[[最近城の北の木が弱っておるのう]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[Spaceキーでいたずらしてはいかんぞ]])
                while m:isRunning() do Yield() end
            elseif (not FlagManager.getFlag(FlagName.kill_bee)) then
                m:streamText("\n"..[[川上の木にハチの巣ができたと聞いておる]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[危ないからSpaceキーで調べてはいかんぞ]])
                while m:isRunning() do Yield() end
            elseif not FlagManager.getFlag(FlagName.kill_slime) then
                m:streamText("\n"..[[皆から感謝されることをしなさい]]..[[そうすれば祈りが届くじゃろう]])
                while m:isRunning() do Yield() end
            else
                m:streamText("\n"..[[わしから言えることはもうない]].."\n"..[[生きるのじゃぞ]])
                while m:isRunning() do Yield() end
            end

        elseif index==1 then
            m:streamText("\n"..[[君は何をしに来たのじゃ]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
