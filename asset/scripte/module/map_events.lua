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
    end,

    house_2 = function (self, e)

    end,


    house_3 = function (self, e)
        local m = MessageWindow.open()
        
        m:streamText([[こんにちは、私は武器屋さんです]])
        while m:isRunning() do Yield() end

        OutLog((FlagName.obtain_tree_branch).."")

        if (FlagManager.getFlag(FlagName.obtain_tree_branch)) and (not FlagManager.getFlag(FlagName.create_wood_sword)) then
            m:streamText("\n"..[[おや、素晴らしい木の枝を]].."\n"..[[お持ちのようですね]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[特別に木刀を作成してあげましょう]])
            while m:isRunning() do Yield() end

            local sleep
            m:streamText("\n"..[[きっこん、かっこん]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[とりゃー、うりゃー]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[ばっこ、ばっこ]])
            while m:isRunning() do Yield() end
            sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end

            m:streamText("\n"..[[お待たせしました]].."\n"..[[木の剣が完成したので差し上げます]])
            while m:isRunning() do Yield() end

            FlagManager.getFlag(FlagName.obtain_tree_branch, false)
            FlagManager.getFlag(FlagName.create_wood_sword, true)
        else
            m:streamText("\n"..[[大変申し訳ございません...]])
            while m:isRunning() do Yield() end
    
            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 0.5) do Yield() end
    
            m:streamText("\n"..[[当店では御用のない方の]].."\n"..[[入店をお断りしています]])
            while m:isRunning() do Yield() end
    
            m:streamText("\n"..[[またのお越しをお待ちしております]])
            while m:isRunning() do Yield() end    
        end

        m:close()
    end,



    tree_1 = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("元気な木が育ってるな")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[おや、枝がもろいぞ]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[そっとする]], [[折っちゃう]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if (index==0) then
            m:streamText("\n"..[[この木も生きているからね]].."\n"..[[そっとしておこう]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[何かに使えそうだからとっておこう]])
            while m:isRunning() do Yield() end
            FlagManager.setFlag(FlagName.obtain_tree_branch, true)
        end

        m:close()
    end,

    tree_on_upstream = function (self, e)
        if FlagManager.getFlag(FlagName.obtain_tree_branch)==true or FlagManager.getFlag(FlagName.create_wood_sword)==true then
            return
        end
        
        local m = MessageWindow.open()
        

        m:streamText("元気な木が育ってるな")
        while m:isRunning() do Yield() end

        m:streamText("\n"..[[おや、枝がもろいぞ]])
        while m:isRunning() do Yield() end


    end
})


