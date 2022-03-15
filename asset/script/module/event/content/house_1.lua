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
            m:streamText("\n"..[[川上の木にハチの巣ができたと聞いておる]])
            while m:isRunning() do Yield() end
            m:streamText("\n"..[[危ないからSpaceキーで調べてはいかんぞ]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[君は何をしに来たのじゃ]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
})
