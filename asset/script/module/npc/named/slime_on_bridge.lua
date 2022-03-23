
slime_on_bridge = {

    new = function()
        
        local self = Instantiate(slime_on_bridge, SlimeLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 15

        return self
    end,

    update = function(self)
        SlimeLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

        if FlagManager.getFlag(FlagName.kill_slime) then
            self.fadeAndDie()
        end

    end,

    move = function (self)
        local x, y, c
        x=self.getX(); y=self.getY();

        c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 10) do Yield() end
            
        while self.doMove(x-16, y) do Yield() end
        while self.doMove(x+16, y) do Yield() end
        while self.doMove(x, y) do Yield() end
    end,

    talk = function (self, e)
        local m = MessageWindow.open()

        m:streamText([[ここは通さないスラ!]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[たたかう]], [[にげる]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            m:close()
            m = MessageWindow.open()

            m:streamText([[何を使おうか?]])
            while m:isRunning() do Yield() end

            local opt = {[[石ころ]]}
            if FlagManager.getFlag(FlagName.have_wood_sword) then
                opt[#opt+1] = [[木の剣]]
            end
            if FlagManager.getFlag(FlagName.obtain_solt) then
                opt[#opt+1] = [[お塩]]
            end
            opt[#opt+1] = [[にげる]]

            s = SelectionWindow.open(opt)
            while s:isRunning() do Yield() end
            local word=s:selectedWord()
            s:close()
            
            if word==[[石ころ]] then
                m:streamText("\n"..[[地面に落ちてた石ころを]].."\n"..[[適当に投げてみよう]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[えいっ!]])
                m:animShake(0.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[ふん、そんなの効かないスラ!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[反撃するスラ!]])
                while m:isRunning() do Yield() end

                m:animFlash()
                while m:isRunning() do Yield() end
            elseif word==[[木の剣]] then
                m:streamText("\n"..[[剣で攻撃だ!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[とりゃっ!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[ぷるん、]])
                while m:isRunning() do Yield() end

                m:streamText([[そんなの効かないスラ!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[反撃するスラ!]])
                while m:isRunning() do Yield() end

                m:animFlash()
                while m:isRunning() do Yield() end
            elseif word==[[お塩]] then
                m:streamText("\n"..[[お塩をかけたら小さくなるはずだ!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[えいっ!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[にゅるにゅる..]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[お、覚えてろスラ!]])
                while m:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.kill_slime, true)
            else
                m:streamText("\n"..[[怖気づいたかスラ!]])
                while m:isRunning() do Yield() end
    
                m:streamText("\n"..[[とっとと消えるスラ!]])
                while m:isRunning() do Yield() end
            end
        elseif index==1 then
            m:streamText("\n"..[[賢明な判断スラ!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[とっとと消えるスラ!]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
    
}











