#
# (C) Copyright 2006, Hisilicon Corp.
# 
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_156MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 156000000" "REG_CONF_PLLFCTRL (0xD09)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_153MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 153000000" "REG_CONF_PLLFCTRL (0xCC9)"


#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_150MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 150000000" "REG_CONF_PLLFCTRL (0xC89)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_147MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 147000000" "REG_CONF_PLLFCTRL (0xC49)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_144MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 144000000" "REG_CONF_PLLFCTRL (0xC09)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_135MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 135000000" "REG_CONF_PLLFCTRL (0xB49)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_132MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 132000000" "REG_CONF_PLLFCTRL (0xB09)"

#!help: For Hi3511V100 ASIC Demo board platform A
hi3511v110Dmeb_126MHZ_config:  unconfig
	@./mkconfig hi3511v110 arm hi3511v100 hi3511v110 NULL NULL \
		"CFG_CLK_MPLLIN 27000000" "CFG_CLK_BUS 126000000" "REG_CONF_PLLFCTRL (0xA89)"


