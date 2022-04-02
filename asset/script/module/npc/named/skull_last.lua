
skull_last = {

    new = function()
        
        local self = Instantiate(skull_last, SkullLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 25
        self.frameInterval = 200

        return self
    end,

    update = function(self)
        SkullLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        local x, y, c
        x=self.getX(); y=self.getY();

        while self.doMove(x-16, y) do Yield() end
        while self.doMove(x+16, y) do Yield() end
        while self.doMove(x, y) do Yield() end
        self.awaits:sleep(4.0)
        while self.doMove(x, y-16) do Yield() end
        while self.doMove(x, y+16) do Yield() end
        while self.doMove(x, y) do Yield() end
        self.awaits:sleep(4.0)

    end,

    talk = function (self, e)
        local index, word
        local m = MessageWindow.open()

        self.awaits:sleep(0.5); Sound.dynamic("mew_boss.mp3")

        self.awaits:streamText(m, [[�ɂ��ɂ��]])
        index, word = self.awaits:selectionWindow({[[�������]], [[�Ȃ�ł�]], [[�ɂႨ��]]})

        if index==0 then
            self.awaits:streamText(m, "\n"..[[���A��k�̒ʂ��Ȃ����]])
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[�����c�b�R�~�ł���]])
        elseif index==2 then
            self.awaits:streamText(m, "\n"..[[�������L�����p������E�҂ł���]])
        end
        self.awaits:streamText(m, "\n"..[[�܂��A����͂�����]].."\n"..[[�����Ă�����̂ł���]])
        index, word = self.awaits:selectionWindow({[[���?]], [[�f��!]]})

        if index==0 then
            self.awaits:streamText(m, "\n"..[[���Ȃ��͂��̐��E���ǂ��v���܂���?]])
            index, word = self.awaits:selectionWindow({[[�~����]], [[����Ȃ�]]})
            if index==0 then
                self.awaits:streamText(m, "\n"..[[�ق��A�����Ԏ��ł���]])    
                self.awaits:streamText(m, "\n"..[[�Ȃ�Ύ��ƈꏏ�ɐ��E�𐪕����܂��傤]])
                index, word = self.awaits:selectionWindow({[[�͂�]], [[������]]})
                if index==0 then
                    self.awaits:streamText(m, "\n"..[[�ӂӂӁA��͂肠�Ȃ���]].."\n"..[[���Ɠ����l���������̂ł���]])
                    self.awaits:streamText(m, "\n"..[[�ł́A���E�𐧂��ɂ����܂��傤��]])
                    m:close()
                    MapEventManager.killPlayer([[�������Đ��E�����̎�ɗ����Ă��܂���..]])
                    return
                elseif index==1 then
                    self.awaits:streamText(m, "\n"..[[������Ƃ����킯�ł���]])
                    self.awaits:streamText(m, "\n"..[[�܂��\�z�͂��Ă��܂���]])
                end
            elseif index==1 then
                self.awaits:streamText(m, "\n"..[[�����ł����A����Ȃ��̂ł���]])
                self.awaits:streamText(m, "\n"..[[�Ȃ�Ύ��ƈꏏ�ɖłڂ��܂��傤]])
                index, word = self.awaits:selectionWindow({[[�͂�]], [[������]]})
                if index==0 then
                    self.awaits:streamText(m, "\n"..[[�ӂӂӁA��͂肠�Ȃ���]].."\n"..[[���Ɠ����l���������̂ł���]])
                    self.awaits:streamText(m, "\n"..[[�ł́A���E���󂵂ɂ����܂��傤��]])
                    m:close()
                    MapEventManager.killPlayer([[�������Đ��E���j�󂳂�Ă��܂���..]])
                    return
                elseif index==1 then
                    self.awaits:streamText(m, "\n"..[[������Ƃ����킯�ł���]])
                    self.awaits:streamText(m, "\n"..[[�܂��\�z�͂��Ă��܂���]])
                end
            end
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[�܂�������͉��������ĂȂ��̂�..]])
            self.awaits:streamText(m, "\n"..[[������̘b�𕷂����������Ȃ��̂ł���]].."\n"..[[�����x���ł�]])
        end

        m:close()
        m = MessageWindow.open()
        
        self.awaits:streamText(m, [[�ł́A��X�͓������R��]].."\n"..[[�������킹�Ă���Ƃ������Ƃ�]].."\n".."��X�͑���Ȃ���΂����܂���")
        index, word = self.awaits:selectionWindow({[[��������]], [[�ɂ���]], [[���̂�]]})

        local enemyAtack = function ()
            self.awaits:streamText(m, "\n"..[[�ł́A���̃^�[���ł�]])
            self.awaits:streamText(m, "\n"..[[�q���[�A�h�J��]])
            --m:streamText("\n"..[[�q���[�A�h�J��]]); 
            m:animFlash()
            while m:isRunning() do Yield() end

            MapEventManager.killPlayer([[���Ə����Ȃ̂ɓ|��Ă��܂���..]])
        end

        if index==0 then
            m:close()
            m = MessageWindow.open()

            m:streamText([[�Ƃ��A]]); m:animShake(1.0)
            while m:isRunning() do Yield() end

            m:streamText([[�����!]]); m:animShake(1.5)
            while m:isRunning() do Yield() end

            m:close()
            m = MessageWindow.open()

            self.awaits:streamText(m, [[�n�n�n�A��������ȍU���œ|����Ƃł�?]])
            enemyAtack()
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[�������܂����?]])
            enemyAtack()
        elseif index==2 then
            m:close()
            m = MessageWindow.open()

            self.awaits:streamText(m, [[�Ƃ肠�����F���Ă݂�]])
            self.awaits:sleep(1.0)
            m:streamText([[..]])
            self.awaits:sleep(1.0)
            while m:isRunning() do Yield() end

            local prayPower = CountFlagForPray()

            if prayPower>=0 then
                self.awaits:streamText(m, "\n"..[[�݂�Ȃ̊��ӂ̋C������������]])
                self.awaits:streamText(m, "\n"..[[�F�肪�͂����悤��]])
                self.awaits:streamText(m, "\n"..[[���ꂪ�F��̗͂�!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:animShake(2.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()
                self.awaits:streamText(m, [[������[!]])
                self.awaits:streamText(m, "\n"..[[�ǁA�ǂ���炠�Ȃ���]].."\n"..[[�����Ă����悤�ł���..]])
                self.fadeAndDie()

                MapEventManager.winningPlayer()
            else
                self.awaits:streamText(m, "\n"..[[�������F�肪�͂��Ȃ�����]])
                m:close()
                m = MessageWindow.open()
                self.awaits:streamText(m, [[�ǂ����c�O�Ȃ悤�ł���]])
                enemyAtack()
            end

        end

        m:close()
    end
    
}








