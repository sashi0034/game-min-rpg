MapEventManager:addMapEvents({

    house_r_1 = function (self, e)
        local m = MessageWindow.open()

        if not FlagManager.getFlag(FlagName.gave_herbs) then
            m:streamText([[ごほん、風邪をひいてしまって]].."\n"..[[こまったねえ]])
            while m:isRunning() do Yield() end

            local table = {[[お薬は?]], [[ばいばい]]}

            if 
                FlagManager.getFlag(FlagName.obtain_herbs_near_bush) and
                FlagManager.getFlag(FlagName.obtain_herbs_near_castle) and
                FlagManager.getFlag(FlagName.obtain_herbs_under_tree)
            then
                table[#table+1] = [[お薬あげる]]
            end

            local s = SelectionWindow.open(table)
            while s:isRunning() do Yield() end
            local word=s:selectedWord()
            s:close()

            local bye = function ()
                m:streamText("\n"..[[ああ、もう帰るのかい]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[また来てくれるとうれしいね]])
                while m:isRunning() do Yield() end
            end
            if word==[[お薬は?]] then
                m:streamText("\n"..[[薬? 無くてこまったねえ、]].."\n"..[[どこかに存在するとは聞くけど..]])
                while m:isRunning() do Yield() end

                local s1 = SelectionWindow.open({[[どこ?]], [[ばいばい]]})
                while s1:isRunning() do Yield() end
                local index1=s1:selectedIndex()
                s1:close()

                if index1==0 then
                    m:streamText("\n"..[[場所ねえ..]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[南の森の3つ並んだ茂みのの間、]].."\n"..[[南の森の最も西の木陰、]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[あとはお城の南西の茂み]])
                    while m:isRunning() do Yield() end

                    m:streamText("\n"..[[このあたりで薬が採れるって聞くねえ]])
                    while m:isRunning() do Yield() end

                    local s2 = SelectionWindow.open({[[任せて]], [[ばいばい]]})
                    while s2:isRunning() do Yield() end
                    local index2=s2:selectedIndex()
                    s2:close()

                    if index2==0 then
                        m:streamText("\n"..[[おや、とってきてくれるのかい]].."\n"..[[頼もしいねえ]])
                        while m:isRunning() do Yield() end
                    elseif index2==1 then
                        bye()
                    end
                elseif index1==1 then
                    bye()
                end
            elseif word==[[お薬あげる]] then
                m:streamText("\n"..[[おや、薬をとってきてくれたのかい]])
                while m:isRunning() do Yield() end
                m:streamText("\n"..[[やさしい子だね、ありがとう]])
                while m:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.gave_herbs, true)
            else
                bye()
            end
        else
            m:streamText([[きみのお陰で助かったよ]].."\n"..[[本当にありがとうね]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
