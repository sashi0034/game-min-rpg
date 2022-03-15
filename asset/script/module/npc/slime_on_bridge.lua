
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
        local w = MessageWindow.open()

        w:streamText([[�����͒ʂ��Ȃ��X��!]])
        while w:isRunning() do Yield() end

        local s = SelectionWindow.open({[[��������]], [[�ɂ���]], [[���̂�]]})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            w:streamText("\n"..[[���̒���������]].."\n"..[[�n�b�s�[�ɂȂ��Ăق����ɂ��]])
            while w:isRunning() do Yield() end
        elseif index==1 then
            w:streamText("\n"..[[�����A�撣���Đ�����ɂ��]])
            while w:isRunning() do Yield() end
        end

        w:close()
    end
    
}











