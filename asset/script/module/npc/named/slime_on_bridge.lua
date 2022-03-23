
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

        m:streamText([[�����͒ʂ��Ȃ��X��!]])
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({[[��������]], [[�ɂ���]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            m:close()
            m = MessageWindow.open()

            m:streamText([[�����g������?]])
            while m:isRunning() do Yield() end

            local opt = {[[�΂���]]}
            if FlagManager.getFlag(FlagName.have_wood_sword) then
                opt[#opt+1] = [[�؂̌�]]
            end
            if FlagManager.getFlag(FlagName.obtain_solt) then
                opt[#opt+1] = [[����]]
            end
            opt[#opt+1] = [[�ɂ���]]

            s = SelectionWindow.open(opt)
            while s:isRunning() do Yield() end
            local word=s:selectedWord()
            s:close()
            
            if word==[[�΂���]] then
                m:streamText("\n"..[[�n�ʂɗ����Ă��΂����]].."\n"..[[�K���ɓ����Ă݂悤]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[������!]])
                m:animShake(0.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[�ӂ�A����Ȃ̌����Ȃ��X��!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[��������X��!]])
                while m:isRunning() do Yield() end

                m:animFlash()
                while m:isRunning() do Yield() end
            elseif word==[[�؂̌�]] then
                m:streamText("\n"..[[���ōU����!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[�Ƃ���!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[�Ղ��A]])
                while m:isRunning() do Yield() end

                m:streamText([[����Ȃ̌����Ȃ��X��!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[��������X��!]])
                while m:isRunning() do Yield() end

                m:animFlash()
                while m:isRunning() do Yield() end
            elseif word==[[����]] then
                m:streamText("\n"..[[�������������珬�����Ȃ�͂���!]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[������!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()

                m:streamText([[�ɂ��ɂ��..]])
                while m:isRunning() do Yield() end

                m:streamText("\n"..[[���A�o���Ă�X��!]])
                while m:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.kill_slime, true)
            else
                m:streamText("\n"..[[�|�C�Â������X��!]])
                while m:isRunning() do Yield() end
    
                m:streamText("\n"..[[�Ƃ��ƂƏ�����X��!]])
                while m:isRunning() do Yield() end
            end
        elseif index==1 then
            m:streamText("\n"..[[�����Ȕ��f�X��!]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[�Ƃ��ƂƏ�����X��!]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end
    
}











