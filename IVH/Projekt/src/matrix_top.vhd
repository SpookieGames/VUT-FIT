----------------------------------------------------------------------------------
-- Engineer: Michal Holesa
-- Module Name: matrix_top - Behavioral
-- Project Name: IVH_projekt
--
-- sysclk (25 MHz) - pin U18, perioda 40 ns
--
-- row(7:0) -- riadkovy vyber: 0 = LED svieti
-- col(7:0) -- stlpcovy vyber: 1 = aktivny stlpec
-- led(3:0) -- debug LED: 0=ST_SLOW 1=ST_FAST 2=ST_DOWN 3=ST_IMG/ROT/ANIM
--
-- JEDNOTKY : counter 
-- DESIATKY : counter  
-- ROM_D : display_data(31:0)
-- ROM_J : display_data(63:32) 
-- BUNKY : 64x cell (for-generate)
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.matrix_pack.ALL;

entity matrix_top is
    port (
        sysclk : in STD_LOGIC;
        row : out STD_LOGIC_VECTOR(7 downto 0);
        col : out STD_LOGIC_VECTOR(7 downto 0);
        led : out STD_LOGIC_VECTOR(3 downto 0);
        rpio_22_r : out STD_LOGIC
    );
end matrix_top;

architecture Behavioral of matrix_top is

    component counter is
        port (
            CLK : in STD_LOGIC;
            RESET : in STD_LOGIC;
            DIR : in STD_LOGIC;
            EN : in STD_LOGIC;
            Q : out STD_LOGIC_VECTOR(3 downto 0);
            EN_OUT : out STD_LOGIC
        );
    end component;

    component bcd_rom is
        port (
            ADDR : in STD_LOGIC_VECTOR(3 downto 0);
            DATA : out STD_LOGIC_VECTOR(31 downto 0)
        );
    end component;

    component cell is
        port (
            BCD_BIT : in STD_LOGIC;
            IMG_BIT : in STD_LOGIC;
            RIGHT : in STD_LOGIC;
            MODE : in STATE_T;
            INV_EN : in STD_LOGIC;
            ANIM_EN : in STD_LOGIC;
            CELL_OUT : out STD_LOGIC
        );
    end component;

    -- Konstanty
    constant C_2MS : natural := 50_000;
    constant C_100MS_TICKS : natural := 50;
    constant C_500MS_DIV : natural := 5;
    constant C_1S_DIV : natural := 2;

    constant C_SMILEY : STD_LOGIC_VECTOR(63 downto 0) :=
        b"00111100_01000010_10010101_10100001" &
        b"10100001_10010101_01000010_00111100";

    -- Typy
    type PHASE_T is (ST_SLOW, ST_FAST, ST_DOWN, ST_IMG, ST_ROT, ST_ANIM);

    -- Power-On Reset
    signal rst_cnt : natural range 0 to 255 := 0;
    signal rst_int : STD_LOGIC := '1';

    -- Generator tickov
    signal cnt_2ms : natural range 0 to C_2MS - 1 := 0;
    signal cnt_100ms : natural range 0 to C_100MS_TICKS - 1 := 0;
    signal cnt_500ms : natural range 0 to C_500MS_DIV - 1 := 0;
    signal cnt_1s : natural range 0 to C_1S_DIV - 1 := 0;

    signal tick_2ms : STD_LOGIC := '0';
    signal tick_100ms : STD_LOGIC := '0';
    signal tick_500ms : STD_LOGIC := '0';
    signal tick_1s : STD_LOGIC := '0';

    -- FSM
    signal phase : PHASE_T := ST_SLOW;
    signal phase_cnt : natural range 0 to 15 := 0;
    signal dir : STD_LOGIC := '0';
    signal reset1 : STD_LOGIC := '1';
    signal reset2 : STD_LOGIC := '1';
    signal img_data : STD_LOGIC_VECTOR(63 downto 0) := C_SMILEY;

    -- Riadenie buniek
    signal en1 : STD_LOGIC := '0';
    signal en2 : STD_LOGIC := '0';
    signal state : STATE_T := S_BCD_NORM;
    signal inv_en : STD_LOGIC := '0';
    signal anim_en : STD_LOGIC := '0';

    -- BCD citace
    signal q1 : STD_LOGIC_VECTOR(3 downto 0);
    signal q2 : STD_LOGIC_VECTOR(3 downto 0);
    signal en1_out : STD_LOGIC;
    signal en2_out : STD_LOGIC;

    -- Datova cesta displeja
    signal display_data : STD_LOGIC_VECTOR(63 downto 0);
    signal cell_out : STD_LOGIC_VECTOR(63 downto 0);

    -- Multiplexing stlpcov
    signal col_idx : unsigned(2 downto 0) := (others => '0');
    signal row_reg : STD_LOGIC_VECTOR(7 downto 0) := (others => '1');

begin

    -- Instancie citacov
    JEDNOTKY : counter port map (
        CLK => sysclk,
        RESET => reset1,
        DIR => dir,
        EN => en1,
        Q => q1,
        EN_OUT => en1_out
    );

    DESIATKY : counter port map (
        CLK => sysclk,
        RESET => reset2,
        DIR => dir,
        EN => en2,
        Q => q2,
        EN_OUT => en2_out
    );

    -- Instancie BCD ROM pamati
    ROM_D : bcd_rom port map (
        ADDR => q2,
        DATA => display_data(31 downto 0)
    );

    ROM_J : bcd_rom port map (
        ADDR => q1,
        DATA => display_data(63 downto 32)
    );

    -- For-generate: 64 kombinacnych buniek
    BUNKY : for i in 0 to 63 generate
        BK : cell port map (
            BCD_BIT => display_data(i),
            IMG_BIT => img_data(i),
            RIGHT => img_data(((i / 8 + 1) mod 8) * 8 + (i mod 8)),
            MODE => state,
            INV_EN => inv_en,
            ANIM_EN => anim_en,
            CELL_OUT => cell_out(i)
        );
    end generate BUNKY;

    P_POR : process(sysclk)
    begin
        if rising_edge(sysclk) then
            if rst_cnt < 255 then
                rst_cnt <= rst_cnt + 1;
                rst_int <= '1';
            else
                rst_int <= '0';
            end if;
        end if;
    end process P_POR;

    -- Generator tick signalov
    P_TICK : process(sysclk)
    begin
        if rising_edge(sysclk) then
            -- Predvolene hodnoty
            tick_2ms <= '0';
            tick_100ms <= '0';
            tick_500ms <= '0';
            tick_1s <= '0';

            if rst_int = '1' then
                cnt_2ms <= 0;
                cnt_100ms <= 0;
                cnt_500ms <= 0;
                cnt_1s <= 0;
            else
                -- 2ms
                if cnt_2ms = C_2MS - 1 then
                    cnt_2ms <= 0;
                    tick_2ms <= '1';

                    -- 100ms
                    if cnt_100ms = C_100MS_TICKS - 1 then
                        cnt_100ms <= 0;
                        tick_100ms <= '1';

                        -- 500ms
                        if cnt_500ms = C_500MS_DIV - 1 then
                            cnt_500ms <= 0;
                            tick_500ms <= '1';

                            -- 1s
                            if cnt_1s = C_1S_DIV - 1 then
                                cnt_1s <= 0;
                                tick_1s <= '1';
                            else
                                cnt_1s <= cnt_1s + 1;
                            end if;

                        else
                            cnt_500ms <= cnt_500ms + 1;
                        end if;

                    else
                        cnt_100ms <= cnt_100ms + 1;
                    end if;

                else
                    cnt_2ms <= cnt_2ms + 1;
                end if;
            end if;
        end if;
    end process P_TICK;

    -- Hlavny stavovy automat
    P_FSM : process(sysclk)
    begin
        if rising_edge(sysclk) then
            
            -- Jednocyklove signaly
            reset1 <= '0';
            reset2 <= '0';

            if rst_int = '1' then
                phase <= ST_SLOW;
                phase_cnt <= 0;
                dir <= '0';
                reset1 <= '1';
                reset2 <= '1';
                img_data <= C_SMILEY;

            else
                case phase is

                    -- ST_SLOW: cita 00 do 10
                    when ST_SLOW =>
                        if q2 = x"1" and q1 = x"0" then
                            phase <= ST_FAST;
                        end if;

                    -- ST_FAST: cita 10 do 99
                    when ST_FAST =>
                        if q2 = x"9" and q1 = x"9" then
                            phase <= ST_DOWN;
                            reset1 <= '1';
                            dir <= '1';
                        end if;

                    -- ST_DOWN: cita 90 do 00 po desiatkach
                    when ST_DOWN =>
                        if q2 = x"0" then
                            phase <= ST_IMG;
                            phase_cnt <= 0;
                            dir <= '0';
                        end if;

                    -- ST_IMG: staticky obrazok smiley
                    when ST_IMG =>
                        if tick_1s = '1' then
                            if phase_cnt = 4 then
                                phase <= ST_ROT;
                                phase_cnt <= 0;
                            else
                                phase_cnt <= phase_cnt + 1;
                            end if;
                        end if;

                    -- ST_ROT: 2x kruhova rotacia obrazka dolava
                    when ST_ROT =>
                        if tick_100ms = '1' then
                            img_data <= img_data(55 downto 0) & img_data(63 downto 56);

                            if phase_cnt = 15 then
                                phase <= ST_ANIM;
                                phase_cnt <= 0;
                            else
                                phase_cnt <= phase_cnt + 1;
                            end if;
                        end if;

                    -- ST_ANIM: 4x bliknutie smiley obrazka 
                    when ST_ANIM =>
                        if tick_100ms = '1' then
                            if phase_cnt mod 2 = 0 then
                                img_data <= (others => '0');
                            else
                                img_data <= C_SMILEY;
                            end if;

                            if phase_cnt = 7 then
                                phase <= ST_SLOW;
                                phase_cnt <= 0;
                                reset1 <= '1';
                                reset2 <= '1';
                                img_data <= C_SMILEY;
                            else
                                phase_cnt <= phase_cnt + 1;
                            end if;
                        end if;

                end case;
            end if;
        end if;
    end process P_FSM;

    -- Riadenie citacov a buniek podla aktualnej fazy
    P_CTRL : process(phase, tick_1s, tick_100ms, tick_500ms, en1_out)
    begin
        en1 <= '0';
        en2 <= '0';
        state <= S_BCD_NORM;
        inv_en <= '0';
        anim_en <= '0';

        case phase is

            when ST_SLOW =>
                en1 <= tick_1s;
                en2 <= en1_out;

            when ST_FAST =>
                en1 <= tick_100ms;
                en2 <= en1_out;

            when ST_DOWN =>
                state <= S_BCD_INV;
                inv_en <= '1';
                en2 <= tick_500ms;

            when ST_IMG =>
                state <= S_ALT;

            when ST_ROT =>
                state <= S_ALT;
                anim_en <= '1';

            when ST_ANIM =>
                state <= S_ALT;

        end case;
    end process P_CTRL;

    -- Posuvanie stlpcoveho indexu
    P_COL_IDX : process(sysclk)
    begin
        if rising_edge(sysclk) then
            if rst_int = '1' then
                col_idx <= (others => '0');
            elsif tick_2ms = '1' then
                col_idx <= col_idx + 1;
            end if;
        end if;
    end process P_COL_IDX;

    -- Vyber riadkovych dat pre aktivny stlpec
    P_MUX : process(col_idx, cell_out)
        variable col_bits : STD_LOGIC_VECTOR(7 downto 0);
        variable col_active : STD_LOGIC;
    begin
        for i in 0 to 7 loop
            col_bits(7-i) := cell_out(to_integer(col_idx) * 8 + i);
        end loop;

        col_active := WLOG_REDUCE(col_bits, col_bits);

        if col_active = '1' then
            row_reg <= not col_bits;
        else
            row_reg <= (others => '1');
        end if;
    end process P_MUX;

    -- Vystupy
    col <= std_logic_vector(shift_left(to_unsigned(1, 8), to_integer(col_idx)));

    row <= row_reg;

    -- Debug LED: signalizuje aktualnu fazu
    led(0) <= '1' when phase = ST_SLOW else '0';
    led(1) <= '1' when phase = ST_FAST else '0';
    led(2) <= '1' when phase = ST_DOWN else '0';
    led(3) <= '1' when phase = ST_IMG  or phase = ST_ROT or phase = ST_ANIM else '0';
    
    rpio_22_r <= '0';

end Behavioral;