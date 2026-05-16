----------------------------------------------------------------------------------
-- Engineer:     Michal Holesa
-- Module Name:  bcd_rom - Behavioral
-- Project Name: IVH_projekt
--
--   ADDR (3:0)  -- BCD cifra 0-9
--   DATA (31:0) -- pixel vzor: 4 stlpce x 8 riadkov
--               -- DATA(7:0)   = stlpec 0 (lavy)
--               -- DATA(15:8)  = stlpec 1
--               -- DATA(23:16) = stlpec 2
--               -- DATA(31:24) = stlpec 3 (pravy)
--               -- V kazdom bajte: bit 0 = riadok 0 (vrch), bit 7 = riadok 7, hodnota 1 = LED svieti
--
-- NEAREST2N z matrix_pack: urcuje hlbku pola ROM
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.matrix_pack.ALL;

entity bcd_rom is
    port (
        ADDR : in  STD_LOGIC_VECTOR(3 downto 0);
        DATA : out STD_LOGIC_VECTOR(31 downto 0)
    );
end bcd_rom;

architecture Behavioral of bcd_rom is
    constant C_ROM_DEPTH : natural := NEAREST2N(10);

    type ROM_T is array(0 to C_ROM_DEPTH - 1) of STD_LOGIC_VECTOR(31 downto 0);
    constant ROM : ROM_T := (
    0  => b"00111110_01000001_01000001_00111110", 
    1  => b"00000000_01000000_01111111_01000010", 
    2  => b"01000110_01001001_01010001_01100010",  
    3  => b"00110110_01001001_01001001_01000001", 
    4  => b"01111111_00001000_00001000_00001111",  
    5  => b"00110001_01001001_01001001_01001111",  
    6  => b"00110000_01001001_01001001_00111110", 
    7  => b"00000011_00001101_01110001_00000001", 
    8  => b"00110110_01001001_01001001_00110110",
    9  => b"00111110_01001001_01001001_00000110",  
    others => (others => '0')
    );

begin
    DATA <= ROM(to_integer(unsigned(ADDR)));

end Behavioral;