----------------------------------------------------------------------------------
-- Engineer:      Michal Holeša
-- Module Name:   bcd_rom_tb - Behavioral
-- Project Name:  IVH_projekt
-- Description:   Testbench bcd_rom
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.matrix_pack.ALL;

entity bcd_rom_tb is
end bcd_rom_tb;

architecture Behavioral of bcd_rom_tb is

    component bcd_rom is
        port (
            ADDR : in  STD_LOGIC_VECTOR(3 downto 0);
            DATA : out STD_LOGIC_VECTOR(31 downto 0)
        );
    end component;

    signal addr_s : STD_LOGIC_VECTOR(3 downto 0) := (others => '0');
    signal data_s : STD_LOGIC_VECTOR(31 downto 0);

    constant T_PROP : time := 10 ns;

    procedure check_digit(
        digit : in natural;
        expected : in STD_LOGIC_VECTOR(31 downto 0);
        signal a : out STD_LOGIC_VECTOR(3 downto 0);
        signal d : in  STD_LOGIC_VECTOR(31 downto 0)
    ) is
    begin
        a <= std_logic_vector(to_unsigned(digit, 4));
        wait for T_PROP;
        assert d = expected
            report "CHYBA: Cifra " & integer'image(digit) & " -- ocakavany a skutocny vzor sa nezhoduju"
            severity error;
    end procedure;

begin

    UUT : bcd_rom port map (
        ADDR => addr_s,
        DATA => data_s
    );

    TB_PROC : process
    begin
        report "bcd_rom_tb: Zaciatok testovania" severity note;

        check_digit(0, STD_LOGIC_VECTOR'(b"00111110_01000001_01000001_00111110"), addr_s, data_s);
        check_digit(1, STD_LOGIC_VECTOR'(b"00000000_01000000_01111111_01000010"), addr_s, data_s);
        check_digit(2, STD_LOGIC_VECTOR'(b"01000110_01001001_01010001_01100010"), addr_s, data_s);
        check_digit(3, STD_LOGIC_VECTOR'(b"00110110_01001001_01001001_01000001"), addr_s, data_s);
        check_digit(4, STD_LOGIC_VECTOR'(b"01111111_00001000_00001000_00001111"), addr_s, data_s);
        check_digit(5, STD_LOGIC_VECTOR'(b"00110001_01001001_01001001_01001111"), addr_s, data_s);
        check_digit(6, STD_LOGIC_VECTOR'(b"00110000_01001001_01001001_00111110"), addr_s, data_s);
        check_digit(7, STD_LOGIC_VECTOR'(b"00000011_00001101_01110001_00000001"), addr_s, data_s);
        check_digit(8, STD_LOGIC_VECTOR'(b"00110110_01001001_01001001_00110110"), addr_s, data_s);
        check_digit(9, STD_LOGIC_VECTOR'(b"00111110_01001001_01001001_00000110"), addr_s, data_s);

        -- Neplatne adresy musia vracat nuly
        for i in 10 to 15 loop
            check_digit(i, STD_LOGIC_VECTOR'(b"00000000_00000000_00000000_00000000"), addr_s, data_s);
        end loop;

        report "bcd_rom_tb: TESTY OK" severity note;
        wait;
    end process TB_PROC;

end Behavioral;