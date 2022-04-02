MapEventManager:addMapEvents({
    tree_on_upstream = function (self, e)
        if (FlagManager.getFlag(FlagName.kill_bee)==true) then
            return
        end
        
        local m = MessageWindow.open()
        
        m:streamText("おや、ハチの巣があるみたいだ")
        while m:isRunning() do Yield() end

        local opt = {[[気にしない]], [[殴る]]}
        
        if FlagManager.getFlag(FlagName.have_wood_sword) then
            opt[#opt+1] = [[木刀]]
        elseif FlagManager.getFlag(FlagName.have_tree_branch) then
            opt[#opt+1] = [[木の枝]]
        end

        local s = SelectionWindow.open(opt)
        while s:isRunning() do Yield() end
        local word=s:selectedWord()
        s:close()

        if (word=="気にしない") then
            m:streamText("\n"..[[危なそうだから]].."\n"..[[見なかったことにしよう]])
            while m:isRunning() do Yield() end
        elseif word=="殴る" then
            m:streamText("\n"..[[えい、]])
            m:animShake(1.0)
            while m:isRunning() do Yield() end

            m:streamText([[えい!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[うわ、蜂さんが出てきた!!]])
            while m:isRunning() do Yield() end

            m:animFlash()
            while m:isRunning() do Yield() end

            MapEventManager.killPlayer([[ハチに刺されてやられちゃった..]])
        elseif word=="木の枝" then
            m:streamText("\n"..[[さっきゲットした枝で攻撃してみよう]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[とりゃ、]])
            while m:isRunning() do Yield() end

            m:streamText([[えい!]])
            m:animShake(1.5)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[うわ、蜂さんが怒ってる!!]])
            while m:isRunning() do Yield() end

            m:animFlash()
            while m:isRunning() do Yield() end
            
            MapEventManager.killPlayer([[ハチに刺されてやられちゃった..]])
        elseif word=="木刀" then
            m:streamText("\n"..[[さっき作ってもらった剣で攻撃してみよう]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[とりゃ、]])
            while m:isRunning() do Yield() end

            m:streamText([[うりゃ!]])
            m:animShake(2.0)
            while m:isRunning() do Yield() end

            local sleep=coroutine.create(self.doSleep)
            while coroutine.resume(sleep, 1.0) do Yield() end
            m:close()
            
            m = MessageWindow.open()
            m:streamText([[よし、蜂さんをやっつけたよ!!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[はちみつを手に入れた]])
            while m:isRunning() do Yield() end
            FlagManager.setFlag(FlagName.kill_bee, true)
            FlagManager.setFlag(FlagName.obtain_honey, true)
        end

        m:close()
    end,
})
    