MapEventManager:addMapEvents({
    house_3 = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[こんにちは、私は武器屋さんです]])
        while m:isRunning() do Yield() end

        if (FlagManager.getFlag(FlagName.have_tree_branch)) and (FlagManager.getFlag(FlagName.have_wood_sword)==false) then
            m:streamText("\n"..[[おや、素晴らしい木の枝を]].."\n"..[[お持ちのようですね]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[特別に木刀を作成してあげましょうか]])
            while m:isRunning() do Yield() end

            local s = SelectionWindow.open({[[うん]], [[だめ]]})
            while s:isRunning() do Yield() end
            local index=s:selectedIndex()
            s:close()

            if (index==0) then
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

                FlagManager.setFlag(FlagName.have_tree_branch, false)
                FlagManager.setFlag(FlagName.have_wood_sword, true)
            elseif index==1 then
                m:streamText("\n"..[[え～]].."\n"..[[けちー!]])
                while m:isRunning() do Yield() end
            end

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
    end})