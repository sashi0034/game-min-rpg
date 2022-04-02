
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

        self.awaits:streamText(m, [[にゃんにゃん]])
        index, word = self.awaits:selectionWindow({[[やっつける]], [[なんでや]], [[にゃおん]]})

        if index==0 then
            self.awaits:streamText(m, "\n"..[[ち、冗談の通じないやつめ]])
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[いいツッコミですね]])
        elseif index==2 then
            self.awaits:streamText(m, "\n"..[[さすが猫耳を継承する勇者ですね]])
        end
        self.awaits:streamText(m, "\n"..[[まあ、それはそうと]].."\n"..[[ある提案があるのですが]])
        index, word = self.awaits:selectionWindow({[[提案?]], [[断る!]]})

        if index==0 then
            self.awaits:streamText(m, "\n"..[[あなたはこの世界をどう思いますか?]])
            index, word = self.awaits:selectionWindow({[[欲しい]], [[いらない]]})
            if index==0 then
                self.awaits:streamText(m, "\n"..[[ほう、いい返事ですね]])    
                self.awaits:streamText(m, "\n"..[[ならば私と一緒に世界を征服しましょう]])
                index, word = self.awaits:selectionWindow({[[はい]], [[いいえ]]})
                if index==0 then
                    self.awaits:streamText(m, "\n"..[[ふふふ、やはりあなたも]].."\n"..[[私と同じ考えだったのですな]])
                    self.awaits:streamText(m, "\n"..[[では、世界を制しにいきましょうか]])
                    m:close()
                    MapEventManager.killPlayer([[こうして世界が魔の手に落ちてしまった..]])
                    return
                elseif index==1 then
                    self.awaits:streamText(m, "\n"..[[交渉決裂というわけですな]])
                    self.awaits:streamText(m, "\n"..[[まあ予想はしていました]])
                end
            elseif index==1 then
                self.awaits:streamText(m, "\n"..[[そうですか、いらないのですか]])
                self.awaits:streamText(m, "\n"..[[ならば私と一緒に滅ぼしましょう]])
                index, word = self.awaits:selectionWindow({[[はい]], [[いいえ]]})
                if index==0 then
                    self.awaits:streamText(m, "\n"..[[ふふふ、やはりあなたも]].."\n"..[[私と同じ考えだったのですな]])
                    self.awaits:streamText(m, "\n"..[[では、世界を壊しにいきましょうか]])
                    m:close()
                    MapEventManager.killPlayer([[こうして世界が破壊されてしまった..]])
                    return
                elseif index==1 then
                    self.awaits:streamText(m, "\n"..[[交渉決裂というわけですな]])
                    self.awaits:streamText(m, "\n"..[[まあ予想はしていました]])
                end
            end
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[まだこちらは何も言ってないのに..]])
            self.awaits:streamText(m, "\n"..[[こちらの話を聞く耳を持たないのですね]].."\n"..[[いい度胸です]])
        end

        m:close()
        m = MessageWindow.open()
        
        self.awaits:streamText(m, [[では、我々は闘う理由を]].."\n"..[[持ちあわせているということで]].."\n".."我々は争わなければいけません")
        index, word = self.awaits:selectionWindow({[[こうげき]], [[にげる]], [[いのる]]})

        local enemyAtack = function ()
            self.awaits:streamText(m, "\n"..[[では、私のターンです]])
            self.awaits:streamText(m, "\n"..[[ヒュー、ドカン]])
            --m:streamText("\n"..[[ヒュー、ドカン]]); 
            m:animFlash()
            while m:isRunning() do Yield() end

            MapEventManager.killPlayer([[あと少しなのに倒れてしまった..]])
        end

        if index==0 then
            m:close()
            m = MessageWindow.open()

            m:streamText([[とりゃ、]]); m:animShake(1.0)
            while m:isRunning() do Yield() end

            m:streamText([[うりゃ!]]); m:animShake(1.5)
            while m:isRunning() do Yield() end

            m:close()
            m = MessageWindow.open()

            self.awaits:streamText(m, [[ハハハ、私をそんな攻撃で倒せるとでも?]])
            enemyAtack()
        elseif index==1 then
            self.awaits:streamText(m, "\n"..[[逃げられませんよ?]])
            enemyAtack()
        elseif index==2 then
            m:close()
            m = MessageWindow.open()

            self.awaits:streamText(m, [[とりあえず祈ってみた]])
            self.awaits:sleep(1.0)
            m:streamText([[..]])
            self.awaits:sleep(1.0)
            while m:isRunning() do Yield() end

            local prayPower = CountFlagForPray()

            if prayPower>=0 then
                self.awaits:streamText(m, "\n"..[[みんなの感謝の気持ちを感じる]])
                self.awaits:streamText(m, "\n"..[[祈りが届いたようだ]])
                self.awaits:streamText(m, "\n"..[[これが祈りの力だ!]])
                m:animShake(1.5)
                while m:isRunning() do Yield() end

                m:animShake(2.5)
                while m:isRunning() do Yield() end

                m:close()
                m = MessageWindow.open()
                self.awaits:streamText(m, [[うぎゃー!]])
                self.awaits:streamText(m, "\n"..[[ど、どうやらあなたを]].."\n"..[[侮っていたようですね..]])
                self.fadeAndDie()

                MapEventManager.winningPlayer()
            else
                self.awaits:streamText(m, "\n"..[[しかし祈りが届かなかった]])
                m:close()
                m = MessageWindow.open()
                self.awaits:streamText(m, [[どうやら残念なようですね]])
                enemyAtack()
            end

        end

        m:close()
    end
    
}








