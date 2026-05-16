-- uart_rx.vhd: UART controller - receiving (RX) side
-- Author(s): Michal Holeša (xholesm00)

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;



-- Entity declaration (DO NOT ALTER THIS PART!)
entity UART_RX is
    port(
        CLK      : in std_logic;
        RST      : in std_logic;
        DIN      : in std_logic;
        DOUT     : out std_logic_vector(7 downto 0);
        DOUT_VLD : out std_logic
    );
end entity;



-- Architecture implementation (INSERT YOUR IMPLEMENTATION HERE)
architecture behavioral of UART_RX is

    -- Interne riadiace signaly
    signal cnt_ce : std_logic;
    signal data_ce : std_logic;
    signal mid : std_logic;

    -- 4-bitovy citac prenosovej rychlosti
    signal cnt : std_logic_vector(3 downto 0) := (others => '0');

    -- 8-bitovy datovy posuvny register
    signal dout_reg : std_logic_vector(7 downto 0) := (others => '0');

begin

    -- Instancia RX FSM
    fsm: entity work.UART_RX_FSM
    port map (
        CLK => CLK,
        RST => RST,
        DIN => DIN,
        MID => mid,
        CNT_CE => cnt_ce,
        DATA_CE => data_ce,
        DOUT_VLD => DOUT_VLD
    );

    -- 4-bitovy citac: pocita 0-15 pocas kazdej periody prenosu
    process(CLK)
    begin
        if rising_edge(CLK) then
            if RST = '1' or cnt_ce = '0' then
                cnt <= (others => '0');
            else
                cnt <= cnt + 1;
            end if;
        end if;
    end process;

    -- Komparator stredu bitu
    -- Aktivuje sa presne na jeden hodinovy takt pri pocte 7
    mid <= '1' when cnt = "0111" else '0';

    -- 8-bitovy posuvny register, LSB ako prvy
    -- Kazdy prichadzajuci bit vstupuje na bit 7 a starsie bity sa posuvaju smerom k bitu 0
    process(CLK)
    begin
        if rising_edge(CLK) then
            if data_ce = '1' then
                dout_reg <= DIN & dout_reg(7 downto 1);
            end if;
        end if;
    end process;

    DOUT <= dout_reg;

end architecture;
