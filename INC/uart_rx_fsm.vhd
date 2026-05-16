-- uart_rx_fsm.vhd: UART controller - finite state machine controlling RX side
-- Author(s): Michal Holeša (xholesm00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity UART_RX_FSM is
    port(
        CLK : in std_logic;
        RST : in std_logic;
        DIN : in std_logic;
        MID : in std_logic;
        CNT_CE : out std_logic;
        DATA_CE : out std_logic;
        DOUT_VLD : out std_logic
    );
end entity;

architecture behavioral of UART_RX_FSM is

    type state_type is (IDLE, START, D0, D1, D2, D3, D4, D5, D6, D7, STOP);
    signal state : state_type := IDLE;

begin

    -- Stavovy register
    -- Synchronne prechody, moznost asynchronneho resetu
    process(CLK)
    begin
        if rising_edge(CLK) then
            if RST = '1' then
                state <= IDLE;
            else
                case state is
                    when IDLE => if DIN = '0' then state <= START; end if;
                    when START => if MID = '1' then state <= D0; end if;
                    when D0 => if MID = '1' then state <= D1; end if;
                    when D1 => if MID = '1' then state <= D2; end if;
                    when D2 => if MID = '1' then state <= D3; end if;
                    when D3 => if MID = '1' then state <= D4; end if;
                    when D4 => if MID = '1' then state <= D5; end if;
                    when D5 => if MID = '1' then state <= D6; end if;
                    when D6 => if MID = '1' then state <= D7; end if;
                    when D7 => if MID = '1' then state <= STOP; end if;
                    when STOP => if MID = '1' then state <= IDLE; end if;
                end case;
            end if;
        end if;
    end process;

    -- Povolenie citaca
    -- Aktivne v kazdom stave okrem IDLE
    CNT_CE <= '0' when state = IDLE else '1';

    -- Povolenie zachytenia dat
    -- Pulz v strede kazdeho datoveho bitu
    DATA_CE <= MID when (state = D0 or state = D1 or state = D2 or state = D3 or
                         state = D4 or state = D5 or state = D6 or state = D7)
               else '0';

    -- Vystup platny
    -- Pulz v strede stop bitu
    DOUT_VLD <= MID when state = STOP else '0';

end architecture;
