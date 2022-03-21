MapEventManager:addMapEvents({

    house_r_2 = function (self, e)
        local m = MessageWindow.open()

        if not FlagManager.getFlag(FlagName.dump_paper_by_eat) then
            if not FlagManager.getFlag(FlagName.try_dump_paper) then
                m:streamText([[この書類をどこかに捨てて]].."\n"..[[きてほしいな]])
                while m:isRunning() do Yield() end
        
                local s = SelectionWindow.open({[[はい]], [[いいえ]]})
                while s:isRunning() do Yield() end
                local index=s:selectedIndex()
                s:close()
        
                if index==0 then
                    m:streamText("\n"..[[頼むね、中身は見ちゃダメだよ]])
                    while m:isRunning() do Yield() end
        
                    FlagManager.setFlag(FlagName.try_dump_paper, true)
                else
                    m:streamText("\n"..[[うげえ、まあいいや]])
                    while m:isRunning() do Yield() end
                end
            else
                m:streamText([[出来たら誰かに食べて]].."\n"..[[もらってきてほしいね]])
                while m:isRunning() do Yield() end
            end
        else
            m:streamText([[書類を捨ててきてくれたのかい]].."\n"..[[ありがとうね～]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
