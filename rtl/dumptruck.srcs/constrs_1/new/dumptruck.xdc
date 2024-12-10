set_max_delay -datapath_only -from [get_cells [list apb1/devinfo/sync_die_serial/sync_en/rx_a_ff_reg \
          apb1/devinfo/sync_idcode/sync_en/rx_a_ff_reg \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[0]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[10]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[1]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[2]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[3]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[4]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[5]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[6]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[7]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[8]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_a_ff_reg[9]} \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/sync_en/rx_a_ff_reg \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[0]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[10]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[1]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[2]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[3]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[4]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[5]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[6]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[7]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[8]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_a_ff_reg[9]} \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/sync_en/rx_a_ff_reg \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[0]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[10]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[11]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[12]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[1]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[2]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[3]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[4]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[5]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[6]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[7]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[8]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_a_ff_reg[9]} \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/sync_en/rx_a_ff_reg \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[0]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[10]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[11]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[12]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[1]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[2]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[3]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[4]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[5]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[6]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[7]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[8]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_a_ff_reg[9]} \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/sync_en/rx_a_ff_reg \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[0]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[10]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[11]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[12]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[1]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[2]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[3]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[4]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[5]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[6]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[7]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[8]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_a_ff_reg[9]} \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_ack/rx_a_ff_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_en/rx_a_ff_reg \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[0]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[10]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[11]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[12]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[1]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[2]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[3]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[4]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[5]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[6]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[7]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[8]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_a_ff_reg[9]} \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_ack/rx_a_ff_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_en/rx_a_ff_reg \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[0]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[1]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[2]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[3]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[4]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_a_ff_reg[5]} \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_ack/rx_a_ff_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_en/rx_a_ff_reg \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[0]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[1]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[2]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[3]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[4]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_a_ff_reg[5]} \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_ack/rx_a_ff_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_en/rx_a_ff_reg \
          {apb2/port_mgmt0/rgmii_bridge/sync_link_speed/reg_a_ff_reg[0]} \
          {apb2/port_mgmt0/rgmii_bridge/sync_link_speed/reg_a_ff_reg[1]} \
          apb2/port_mgmt0/rgmii_bridge/sync_link_speed/sync_en/rx_a_ff_reg]] -to [get_cells [list {apb1/devinfo/sync_die_serial/reg_b_reg[0]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[10]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[11]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[12]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[13]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[14]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[15]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[16]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[17]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[18]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[19]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[1]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[20]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[21]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[22]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[23]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[24]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[25]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[26]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[27]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[28]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[29]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[2]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[30]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[31]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[32]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[33]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[34]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[35]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[36]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[37]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[38]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[39]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[3]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[40]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[41]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[42]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[43]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[44]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[45]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[46]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[47]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[48]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[49]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[4]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[50]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[51]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[52]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[53]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[54]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[55]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[56]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[57]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[58]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[59]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[5]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[60]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[61]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[62]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[63]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[6]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[7]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[8]} \
          {apb1/devinfo/sync_die_serial/reg_b_reg[9]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[0]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[10]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[11]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[12]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[13]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[14]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[15]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[16]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[17]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[18]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[19]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[1]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[20]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[21]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[22]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[23]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[24]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[25]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[26]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[27]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[28]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[29]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[2]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[30]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[31]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[3]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[4]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[5]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[6]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[7]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[8]} \
          {apb1/devinfo/sync_idcode/reg_b_reg[9]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[0]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[10]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[1]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[2]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[3]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[4]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[5]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[6]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[7]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[8]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/reg_b_reg[9]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[0]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[10]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[1]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[2]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[3]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[4]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[5]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[6]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[7]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[8]} \
          {apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/reg_b_reg[9]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[0]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[10]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[11]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[12]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[1]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[2]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[3]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[4]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[5]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[6]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[7]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[8]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/reg_b_reg[9]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[0]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[10]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[11]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[12]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[1]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[2]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[3]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[4]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[5]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[6]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[7]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[8]} \
          {apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/reg_b_reg[9]} \
          {apb2/eth_tx_fifo/sync_link_up/reg_b[12]_i_1__0} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[0]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[10]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[11]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[12]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[1]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[2]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[3]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[4]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[5]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[6]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[7]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[8]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/reg_b_reg[9]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[0]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[10]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[11]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[12]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[1]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[2]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[3]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[4]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[5]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[6]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[7]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[8]} \
          {apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/reg_b_reg[9]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[0]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[1]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[2]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[3]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[4]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/reg_b_reg[5]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[0]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[1]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[2]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[3]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[4]} \
          {apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/reg_b_reg[5]} \
          {apb2/port_mgmt0/rgmii_bridge/sync_link_speed/reg_b_reg[0]} \
          {apb2/port_mgmt0/rgmii_bridge/sync_link_speed/reg_b_reg[1]} \
          {apb2/sync_rgmii_link_up/reg_b[12]_i_1}]] 4.000
set_max_delay -from [get_cells [list apb1/devinfo/sync_die_serial/sync_ack/sync/dout0_reg \
          apb1/devinfo/sync_die_serial/sync_en/sync/dout0_reg \
          apb1/devinfo/sync_idcode/sync_ack/sync/dout0_reg \
          apb1/devinfo/sync_idcode/sync_en/sync/dout0_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/sync_ack/sync/dout0_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/sync_en/sync/dout0_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/sync_ack/sync/dout0_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/sync_en/sync/dout0_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/sync_ack/sync/dout0_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/sync_en/sync/dout0_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/sync_ack/sync/dout0_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/sync_en/sync/dout0_reg \
          apb2/eth_tx_fifo/sync_link_up/dout0_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_ack/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_en/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_ack/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_en/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_ack/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_en/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_ack/sync/dout0_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_en/sync/dout0_reg \
          apb2/port_mgmt0/mac/sync_link_speed_0/dout0_reg \
          apb2/port_mgmt0/mac/sync_link_speed_1/dout0_reg \
          apb2/port_mgmt0/rgmii_bridge/sync_link_speed/sync_ack/sync/dout0_reg \
          apb2/port_mgmt0/rgmii_bridge/sync_link_speed/sync_en/sync/dout0_reg \
          apb2/sync_rgmii_link_up/dout0_reg]] -to [get_cells [list apb1/devinfo/sync_die_serial/sync_ack/sync/dout1_reg \
          apb1/devinfo/sync_die_serial/sync_en/sync/dout1_reg \
          apb1/devinfo/sync_idcode/sync_ack/sync/dout1_reg \
          apb1/devinfo/sync_idcode/sync_en/sync/dout1_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/sync_ack/sync/dout1_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_head/sync_en/sync/dout1_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/sync_ack/sync/dout1_reg \
          apb2/eth_rx_cdc/rx_cdc_fifo/sync_tail/sync_en/sync/dout1_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/sync_ack/sync/dout1_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_head/sync_en/sync/dout1_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/sync_ack/sync/dout1_reg \
          apb2/eth_rx_fifo/rx_cdc_fifo/sync_tail/sync_en/sync/dout1_reg \
          apb2/eth_tx_fifo/sync_link_up/dout1_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_ack/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_rd_ptr/sync_en/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_ack/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_cdc_fifo/sync_wr_ptr/sync_en/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_ack/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_rd_ptr/sync_en/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_ack/sync/dout1_reg \
          apb2/eth_tx_fifo/tx_framelen_fifo/sync_wr_ptr/sync_en/sync/dout1_reg \
          apb2/port_mgmt0/mac/sync_link_speed_0/dout1_reg \
          apb2/port_mgmt0/mac/sync_link_speed_1/dout1_reg \
          apb2/port_mgmt0/rgmii_bridge/sync_link_speed/sync_ack/sync/dout1_reg \
          apb2/port_mgmt0/rgmii_bridge/sync_link_speed/sync_en/sync/dout1_reg \
          apb2/sync_rgmii_link_up/dout1_reg]] 4.000
########################################################################################################################
# IO pinout constraints

set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[49]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[48]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[47]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[46]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[45]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[44]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[43]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[42]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[41]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[40]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[39]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[38]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[37]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[36]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[35]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[34]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[33]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[32]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[31]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[30]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[29]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[28]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[27]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[26]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[25]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[24]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[23]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[22]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[21]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[20]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[19]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[18]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[17]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[16]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[15]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[14]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[13]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[12]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[11]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {bank16_io_3v3[0]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[49]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[48]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[47]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[46]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[45]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[44]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[43]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[42]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[41]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[40]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[39]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[38]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[37]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[36]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[35]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[34]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[33]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[32]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[31]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[30]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[29]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[28]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[27]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[26]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[25]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[24]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[23]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[22]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[21]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[20]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[19]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[18]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[17]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[16]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[15]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[14]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[13]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[12]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[11]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[10]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[9]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[8]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[7]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[6]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[5]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[4]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[3]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[2]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[1]}]
set_property IOSTANDARD LVCMOS12 [get_ports {bank34_io_1v2[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[49]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[48]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[47]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[46]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[45]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[44]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[43]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[42]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[41]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[40]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[39]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[38]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[37]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[36]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[35]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[34]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[33]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[32]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[31]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[30]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[29]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[28]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[27]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[26]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[25]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[24]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[23]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[22]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[21]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[20]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[19]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[18]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[17]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[16]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[15]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[14]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[13]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[12]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[11]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[10]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[9]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[8]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[7]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[6]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[5]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[4]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {bank35_io_1v8[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[49]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[48]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[47]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[46]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[45]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[44]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[43]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[42]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[41]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[40]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[39]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[38]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[37]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[36]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[35]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[34]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[33]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[32]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[31]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[30]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[29]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[28]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[27]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[26]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {bank36_io_2v5[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[15]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[14]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[13]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[12]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[11]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_ad[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_nbl[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_nbl[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {pmod_gpio[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {flash_dq[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {flash_dq[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {flash_dq[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {flash_dq[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {eth_led_in[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {eth_led_in[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {eth_led_out[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {eth_led_out[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[7]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[6]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[5]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[4]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {ram_dq[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_rxd[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_rxd[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_rxd[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_rxd[0]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_txd[3]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_txd[2]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_txd[1]}]
set_property IOSTANDARD LVCMOS18 [get_ports {rgmii_txd[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports clk_25mhz]
set_property IOSTANDARD LVCMOS18 [get_ports eth_mdc]
set_property IOSTANDARD LVCMOS18 [get_ports eth_mdio]
set_property IOSTANDARD LVCMOS18 [get_ports eth_rst_n]
set_property IOSTANDARD LVCMOS18 [get_ports flash_cs_n]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_clk]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_ne1]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_ne3]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_nl_nadv]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_nwait]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_noe]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_nwe]
set_property IOSTANDARD LVCMOS33 [get_ports led_ctrl]
set_property IOSTANDARD LVCMOS18 [get_ports ram_rst_n]
set_property IOSTANDARD LVCMOS18 [get_ports rgmii_rx_clk]
set_property IOSTANDARD LVCMOS18 [get_ports rgmii_rx_dv]
set_property IOSTANDARD LVCMOS18 [get_ports rgmii_tx_clk]
set_property IOSTANDARD LVCMOS18 [get_ports rgmii_tx_en]
set_property PACKAGE_PIN G13 [get_ports {bank16_io_3v3[9]}]
set_property PACKAGE_PIN G11 [get_ports {bank16_io_3v3[2]}]
set_property PACKAGE_PIN F11 [get_ports {bank16_io_3v3[5]}]
set_property PACKAGE_PIN G10 [get_ports {bank16_io_3v3[0]}]
set_property PACKAGE_PIN F10 [get_ports {bank16_io_3v3[3]}]
set_property PACKAGE_PIN E11 [get_ports {bank16_io_3v3[19]}]
set_property PACKAGE_PIN D11 [get_ports {bank16_io_3v3[16]}]
set_property PACKAGE_PIN F13 [get_ports {bank16_io_3v3[11]}]
set_property PACKAGE_PIN F14 [get_ports {bank16_io_3v3[15]}]
set_property PACKAGE_PIN F12 [get_ports {bank16_io_3v3[4]}]
set_property PACKAGE_PIN E12 [get_ports {bank16_io_3v3[7]}]
set_property PACKAGE_PIN G14 [get_ports {bank16_io_3v3[13]}]
set_property PACKAGE_PIN F15 [get_ports {bank16_io_3v3[17]}]
set_property PACKAGE_PIN D13 [get_ports {bank16_io_3v3[20]}]
set_property PACKAGE_PIN C13 [get_ports {bank16_io_3v3[21]}]
set_property PACKAGE_PIN A11 [get_ports {bank16_io_3v3[23]}]
set_property PACKAGE_PIN A12 [get_ports {bank16_io_3v3[27]}]
set_property PACKAGE_PIN B13 [get_ports {bank16_io_3v3[33]}]
set_property PACKAGE_PIN A13 [get_ports {bank16_io_3v3[31]}]
set_property PACKAGE_PIN C10 [get_ports {bank16_io_3v3[1]}]
set_property PACKAGE_PIN C11 [get_ports {bank16_io_3v3[25]}]
set_property PACKAGE_PIN D14 [get_ports {bank16_io_3v3[22]}]
set_property PACKAGE_PIN D15 [get_ports {bank16_io_3v3[26]}]
set_property PACKAGE_PIN D12 [get_ports {bank16_io_3v3[18]}]
set_property PACKAGE_PIN C12 [get_ports {bank16_io_3v3[29]}]
set_property PACKAGE_PIN C15 [get_ports {bank16_io_3v3[24]}]
set_property PACKAGE_PIN C16 [get_ports {bank16_io_3v3[28]}]
set_property PACKAGE_PIN B15 [get_ports {bank16_io_3v3[39]}]
set_property PACKAGE_PIN B16 [get_ports {bank16_io_3v3[43]}]
set_property PACKAGE_PIN D16 [get_ports {bank16_io_3v3[30]}]
set_property PACKAGE_PIN C17 [get_ports {bank16_io_3v3[47]}]
set_property PACKAGE_PIN E15 [get_ports {bank16_io_3v3[6]}]
set_property PACKAGE_PIN E16 [get_ports {bank16_io_3v3[10]}]
set_property PACKAGE_PIN B14 [get_ports {bank16_io_3v3[37]}]
set_property PACKAGE_PIN A14 [get_ports {bank16_io_3v3[35]}]
set_property PACKAGE_PIN A16 [get_ports {bank16_io_3v3[41]}]
set_property PACKAGE_PIN A17 [get_ports {bank16_io_3v3[45]}]
set_property PACKAGE_PIN D18 [get_ports {bank16_io_3v3[34]}]
set_property PACKAGE_PIN C18 [get_ports {bank16_io_3v3[40]}]
set_property PACKAGE_PIN C19 [get_ports {bank16_io_3v3[36]}]
set_property PACKAGE_PIN B19 [get_ports {bank16_io_3v3[44]}]
set_property PACKAGE_PIN A18 [get_ports {bank16_io_3v3[49]}]
set_property PACKAGE_PIN A19 [get_ports {bank16_io_3v3[42]}]
set_property PACKAGE_PIN E17 [get_ports {bank16_io_3v3[32]}]
set_property PACKAGE_PIN E18 [get_ports {bank16_io_3v3[12]}]
set_property PACKAGE_PIN B20 [get_ports {bank16_io_3v3[48]}]
set_property PACKAGE_PIN A20 [get_ports {bank16_io_3v3[46]}]
set_property PACKAGE_PIN E19 [get_ports {bank16_io_3v3[14]}]
set_property PACKAGE_PIN D19 [get_ports {bank16_io_3v3[38]}]
set_property PACKAGE_PIN G15 [get_ports {bank16_io_3v3[8]}]
set_property PACKAGE_PIN E8 [get_ports {bank36_io_2v5[18]}]
set_property PACKAGE_PIN B9 [get_ports {bank36_io_2v5[34]}]
set_property PACKAGE_PIN A9 [get_ports {bank36_io_2v5[36]}]
set_property PACKAGE_PIN B10 [get_ports {bank36_io_2v5[30]}]
set_property PACKAGE_PIN A10 [get_ports {bank36_io_2v5[32]}]
set_property PACKAGE_PIN C9 [get_ports {bank36_io_2v5[20]}]
set_property PACKAGE_PIN C8 [get_ports {bank36_io_2v5[16]}]
set_property PACKAGE_PIN A7 [get_ports {bank36_io_2v5[42]}]
set_property PACKAGE_PIN A6 [get_ports {bank36_io_2v5[46]}]
set_property PACKAGE_PIN B8 [get_ports {bank36_io_2v5[38]}]
set_property PACKAGE_PIN B7 [get_ports {bank36_io_2v5[40]}]
set_property PACKAGE_PIN D8 [get_ports {bank36_io_2v5[14]}]
set_property PACKAGE_PIN D7 [get_ports {bank36_io_2v5[12]}]
set_property PACKAGE_PIN C6 [get_ports {bank36_io_2v5[10]}]
set_property PACKAGE_PIN C5 [get_ports {bank36_io_2v5[48]}]
set_property PACKAGE_PIN D6 [get_ports {bank36_io_2v5[8]}]
set_property PACKAGE_PIN D5 [get_ports {bank36_io_2v5[17]}]
set_property PACKAGE_PIN G8 [get_ports {bank36_io_2v5[0]}]
set_property PACKAGE_PIN F8 [get_ports {bank36_io_2v5[6]}]
set_property PACKAGE_PIN F5 [get_ports {bank36_io_2v5[19]}]
set_property PACKAGE_PIN E5 [get_ports {bank36_io_2v5[13]}]
set_property PACKAGE_PIN G7 [get_ports {bank36_io_2v5[1]}]
set_property PACKAGE_PIN G6 [get_ports {bank36_io_2v5[3]}]
set_property PACKAGE_PIN F7 [get_ports {bank36_io_2v5[4]}]
set_property PACKAGE_PIN F6 [get_ports {bank36_io_2v5[2]}]
set_property PACKAGE_PIN B6 [get_ports {bank36_io_2v5[44]}]
set_property PACKAGE_PIN A5 [get_ports {bank36_io_2v5[28]}]
set_property PACKAGE_PIN A4 [get_ports {bank36_io_2v5[24]}]
set_property PACKAGE_PIN A3 [get_ports {bank36_io_2v5[21]}]
set_property PACKAGE_PIN B3 [get_ports {bank36_io_2v5[23]}]
set_property PACKAGE_PIN A2 [get_ports {bank36_io_2v5[25]}]
set_property PACKAGE_PIN C4 [get_ports {bank36_io_2v5[26]}]
set_property PACKAGE_PIN C3 [get_ports {bank36_io_2v5[22]}]
set_property PACKAGE_PIN B2 [get_ports {bank36_io_2v5[49]}]
set_property PACKAGE_PIN B1 [get_ports {bank36_io_2v5[47]}]
set_property PACKAGE_PIN C2 [get_ports {bank36_io_2v5[45]}]
set_property PACKAGE_PIN C1 [get_ports {bank36_io_2v5[43]}]
set_property PACKAGE_PIN G4 [get_ports {bank36_io_2v5[7]}]
set_property PACKAGE_PIN G3 [get_ports {bank36_io_2v5[5]}]
set_property PACKAGE_PIN E4 [get_ports {bank36_io_2v5[11]}]
set_property PACKAGE_PIN D4 [get_ports {bank36_io_2v5[15]}]
set_property PACKAGE_PIN F1 [get_ports {bank36_io_2v5[31]}]
set_property PACKAGE_PIN E1 [get_ports {bank36_io_2v5[35]}]
set_property PACKAGE_PIN G2 [get_ports {bank36_io_2v5[29]}]
set_property PACKAGE_PIN G1 [get_ports {bank36_io_2v5[27]}]
set_property PACKAGE_PIN E3 [get_ports {bank36_io_2v5[37]}]
set_property PACKAGE_PIN D2 [get_ports {bank36_io_2v5[41]}]
set_property PACKAGE_PIN E2 [get_ports {bank36_io_2v5[33]}]
set_property PACKAGE_PIN D1 [get_ports {bank36_io_2v5[39]}]
set_property PACKAGE_PIN F4 [get_ports {bank36_io_2v5[9]}]
set_property PACKAGE_PIN H7 [get_ports {bank35_io_1v8[22]}]
set_property PACKAGE_PIN J3 [get_ports {bank35_io_1v8[34]}]
set_property PACKAGE_PIN H2 [get_ports {bank35_io_1v8[40]}]
set_property PACKAGE_PIN H4 [get_ports {bank35_io_1v8[38]}]
set_property PACKAGE_PIN H3 [get_ports {bank35_io_1v8[36]}]
set_property PACKAGE_PIN J2 [get_ports {bank35_io_1v8[48]}]
set_property PACKAGE_PIN J1 [get_ports {bank35_io_1v8[46]}]
set_property PACKAGE_PIN K3 [get_ports {bank35_io_1v8[41]}]
set_property PACKAGE_PIN K2 [get_ports {bank35_io_1v8[44]}]
set_property PACKAGE_PIN H6 [get_ports {bank35_io_1v8[26]}]
set_property PACKAGE_PIN H5 [get_ports {bank35_io_1v8[24]}]
set_property PACKAGE_PIN K6 [get_ports {bank35_io_1v8[16]}]
set_property PACKAGE_PIN J6 [get_ports {bank35_io_1v8[20]}]
set_property PACKAGE_PIN J8 [get_ports {bank35_io_1v8[14]}]
set_property PACKAGE_PIN J7 [get_ports {bank35_io_1v8[18]}]
set_property PACKAGE_PIN L7 [get_ports {bank35_io_1v8[3]}]
set_property PACKAGE_PIN L6 [get_ports {bank35_io_1v8[0]}]
set_property PACKAGE_PIN L5 [get_ports {bank35_io_1v8[1]}]
set_property PACKAGE_PIN K5 [get_ports {bank35_io_1v8[32]}]
set_property PACKAGE_PIN L4 [get_ports {bank35_io_1v8[28]}]
set_property PACKAGE_PIN K4 [get_ports {bank35_io_1v8[30]}]
set_property PACKAGE_PIN L8 [get_ports {bank35_io_1v8[10]}]
set_property PACKAGE_PIN K8 [get_ports {bank35_io_1v8[12]}]
set_property PACKAGE_PIN M8 [get_ports {bank35_io_1v8[8]}]
set_property PACKAGE_PIN M7 [get_ports {bank35_io_1v8[5]}]
set_property PACKAGE_PIN M4 [get_ports {bank35_io_1v8[27]}]
set_property PACKAGE_PIN M3 [get_ports {bank35_io_1v8[45]}]
set_property PACKAGE_PIN L1 [get_ports {bank35_io_1v8[43]}]
set_property PACKAGE_PIN K1 [get_ports {bank35_io_1v8[42]}]
set_property PACKAGE_PIN M2 [get_ports {bank35_io_1v8[49]}]
set_property PACKAGE_PIN M1 [get_ports {bank35_io_1v8[47]}]
set_property PACKAGE_PIN N4 [get_ports {bank35_io_1v8[29]}]
set_property PACKAGE_PIN N3 [get_ports {bank35_io_1v8[31]}]
set_property PACKAGE_PIN P1 [get_ports {bank35_io_1v8[35]}]
set_property PACKAGE_PIN N1 [get_ports {bank35_io_1v8[39]}]
set_property PACKAGE_PIN P3 [get_ports {bank35_io_1v8[33]}]
set_property PACKAGE_PIN P2 [get_ports {bank35_io_1v8[37]}]
set_property PACKAGE_PIN P8 [get_ports {bank35_io_1v8[4]}]
set_property PACKAGE_PIN N8 [get_ports {bank35_io_1v8[6]}]
set_property PACKAGE_PIN N5 [get_ports {bank35_io_1v8[23]}]
set_property PACKAGE_PIN M5 [get_ports {bank35_io_1v8[25]}]
set_property PACKAGE_PIN P6 [get_ports {bank35_io_1v8[13]}]
set_property PACKAGE_PIN N6 [get_ports {bank35_io_1v8[17]}]
set_property PACKAGE_PIN P7 [get_ports {bank35_io_1v8[15]}]
set_property PACKAGE_PIN N7 [get_ports {bank35_io_1v8[7]}]
set_property PACKAGE_PIN R7 [get_ports {bank35_io_1v8[2]}]
set_property PACKAGE_PIN R6 [get_ports {bank35_io_1v8[11]}]
set_property PACKAGE_PIN R5 [get_ports {bank35_io_1v8[9]}]
set_property PACKAGE_PIN R4 [get_ports {bank35_io_1v8[21]}]
set_property PACKAGE_PIN R3 [get_ports {bank35_io_1v8[19]}]
set_property PACKAGE_PIN R2 [get_ports {bank34_io_1v2[24]}]
set_property PACKAGE_PIN W5 [get_ports {bank34_io_1v2[16]}]
set_property PACKAGE_PIN W4 [get_ports {bank34_io_1v2[14]}]
set_property PACKAGE_PIN T7 [get_ports {bank34_io_1v2[1]}]
set_property PACKAGE_PIN T6 [get_ports {bank34_io_1v2[0]}]
set_property PACKAGE_PIN V5 [get_ports {bank34_io_1v2[12]}]
set_property PACKAGE_PIN V4 [get_ports {bank34_io_1v2[10]}]
set_property PACKAGE_PIN T5 [get_ports {bank34_io_1v2[2]}]
set_property PACKAGE_PIN U5 [get_ports {bank34_io_1v2[3]}]
set_property PACKAGE_PIN V7 [get_ports {bank34_io_1v2[20]}]
set_property PACKAGE_PIN V6 [get_ports {bank34_io_1v2[18]}]
set_property PACKAGE_PIN T8 [get_ports {bank34_io_1v2[5]}]
set_property PACKAGE_PIN U8 [get_ports {bank34_io_1v2[7]}]
set_property PACKAGE_PIN V1 [get_ports {bank34_io_1v2[34]}]
set_property PACKAGE_PIN W1 [get_ports {bank34_io_1v2[38]}]
set_property PACKAGE_PIN W2 [get_ports {bank34_io_1v2[36]}]
set_property PACKAGE_PIN Y1 [get_ports {bank34_io_1v2[42]}]
set_property PACKAGE_PIN U2 [get_ports {bank34_io_1v2[32]}]
set_property PACKAGE_PIN U1 [get_ports {bank34_io_1v2[30]}]
set_property PACKAGE_PIN W3 [get_ports {bank34_io_1v2[40]}]
set_property PACKAGE_PIN Y3 [get_ports {bank34_io_1v2[44]}]
set_property PACKAGE_PIN U4 [get_ports {bank34_io_1v2[6]}]
set_property PACKAGE_PIN U3 [get_ports {bank34_io_1v2[8]}]
set_property PACKAGE_PIN T3 [get_ports {bank34_io_1v2[4]}]
set_property PACKAGE_PIN T2 [get_ports {bank34_io_1v2[28]}]
set_property PACKAGE_PIN AA6 [get_ports {bank34_io_1v2[47]}]
set_property PACKAGE_PIN AB6 [get_ports {bank34_io_1v2[49]}]
set_property PACKAGE_PIN Y6 [get_ports {bank34_io_1v2[9]}]
set_property PACKAGE_PIN Y5 [get_ports {bank34_io_1v2[29]}]
set_property PACKAGE_PIN Y4 [get_ports {bank34_io_1v2[25]}]
set_property PACKAGE_PIN AA3 [get_ports {bank34_io_1v2[21]}]
set_property PACKAGE_PIN AB3 [get_ports {bank34_io_1v2[23]}]
set_property PACKAGE_PIN AB2 [get_ports {bank34_io_1v2[22]}]
set_property PACKAGE_PIN AA2 [get_ports {bank34_io_1v2[48]}]
set_property PACKAGE_PIN AA1 [get_ports {bank34_io_1v2[46]}]
set_property PACKAGE_PIN AB5 [get_ports {bank34_io_1v2[43]}]
set_property PACKAGE_PIN AB4 [get_ports {bank34_io_1v2[27]}]
set_property PACKAGE_PIN V8 [get_ports {bank34_io_1v2[19]}]
set_property PACKAGE_PIN W8 [get_ports {bank34_io_1v2[15]}]
set_property PACKAGE_PIN Y8 [get_ports {bank34_io_1v2[17]}]
set_property PACKAGE_PIN AA8 [get_ports {bank34_io_1v2[39]}]
set_property PACKAGE_PIN W7 [get_ports {bank34_io_1v2[11]}]
set_property PACKAGE_PIN Y7 [get_ports {bank34_io_1v2[13]}]
set_property PACKAGE_PIN AA10 [get_ports {bank34_io_1v2[33]}]
set_property PACKAGE_PIN AB10 [get_ports {bank34_io_1v2[31]}]
set_property PACKAGE_PIN AA7 [get_ports {bank34_io_1v2[45]}]
set_property PACKAGE_PIN AB7 [get_ports {bank34_io_1v2[41]}]
set_property PACKAGE_PIN AA9 [get_ports {bank34_io_1v2[37]}]
set_property PACKAGE_PIN AB9 [get_ports {bank34_io_1v2[35]}]
set_property PACKAGE_PIN T1 [get_ports {bank34_io_1v2[26]}]
set_property PACKAGE_PIN A21 [get_ports {fmc_a_hi[2]}]
set_property PACKAGE_PIN F17 [get_ports {fmc_ad[11]}]
set_property PACKAGE_PIN F18 [get_ports {fmc_ad[13]}]
set_property PACKAGE_PIN H17 [get_ports {fmc_ad[12]}]
set_property PACKAGE_PIN G17 [get_ports {fmc_ad[10]}]
set_property PACKAGE_PIN J17 [get_ports {fmc_ad[9]}]
set_property PACKAGE_PIN H18 [get_ports {fmc_ad[5]}]
set_property PACKAGE_PIN H16 [get_ports {fmc_ad[6]}]
set_property PACKAGE_PIN G16 [get_ports {fmc_ad[8]}]
set_property PACKAGE_PIN K16 [get_ports {pmod_gpio[4]}]
set_property PACKAGE_PIN J16 [get_ports {fmc_ad[4]}]
set_property PACKAGE_PIN K15 [get_ports led_ctrl]
set_property PACKAGE_PIN J15 [get_ports {fmc_ad[7]}]
set_property PACKAGE_PIN B21 [get_ports {fmc_ad[0]}]
set_property PACKAGE_PIN B22 [get_ports fmc_ne3]
set_property PACKAGE_PIN D20 [get_ports fmc_nwe]
set_property PACKAGE_PIN C20 [get_ports fmc_ne1]
set_property PACKAGE_PIN D21 [get_ports {fmc_ad[2]}]
set_property PACKAGE_PIN C22 [get_ports fmc_nwait]
set_property PACKAGE_PIN D22 [get_ports {fmc_ad[3]}]
set_property PACKAGE_PIN E22 [get_ports fmc_ne2]
set_property PACKAGE_PIN F21 [get_ports {fmc_a_hi[8]}]
set_property PACKAGE_PIN F22 [get_ports fmc_nl_nadv]
set_property PACKAGE_PIN F19 [get_ports fmc_clk]
set_property PACKAGE_PIN F20 [get_ports {fmc_ad[15]}]
set_property PACKAGE_PIN M15 [get_ports clk_25mhz]
set_property PACKAGE_PIN L15 [get_ports {led[0]}]
set_property PACKAGE_PIN M17 [get_ports {led[3]}]
set_property PACKAGE_PIN M18 [get_ports {pmod_gpio[3]}]
set_property PACKAGE_PIN L16 [get_ports {led[1]}]
set_property PACKAGE_PIN K17 [get_ports {pmod_gpio[0]}]
set_property PACKAGE_PIN K18 [get_ports {fmc_a_hi[9]}]
set_property PACKAGE_PIN K19 [get_ports {fmc_a_hi[6]}]
set_property PACKAGE_PIN J19 [get_ports {fmc_nbl[1]}]
set_property PACKAGE_PIN J20 [get_ports {fmc_a_hi[0]}]
set_property PACKAGE_PIN L18 [get_ports {pmod_gpio[5]}]
set_property PACKAGE_PIN L19 [get_ports {pmod_gpio[7]}]
set_property PACKAGE_PIN H19 [get_ports {fmc_a_hi[1]}]
set_property PACKAGE_PIN G20 [get_ports {fmc_ad[14]}]
set_property PACKAGE_PIN G21 [get_ports {fmc_nbl[0]}]
set_property PACKAGE_PIN G22 [get_ports {fmc_a_hi[7]}]
set_property PACKAGE_PIN H20 [get_ports fmc_noe]
set_property PACKAGE_PIN H21 [get_ports {fmc_a_hi[3]}]
set_property PACKAGE_PIN L22 [get_ports {pmod_gpio[1]}]
set_property PACKAGE_PIN K22 [get_ports {fmc_ad[1]}]
set_property PACKAGE_PIN J21 [get_ports {fmc_a_hi[4]}]
set_property PACKAGE_PIN J22 [get_ports {fmc_a_hi[5]}]
set_property PACKAGE_PIN L20 [get_ports {pmod_gpio[2]}]
set_property PACKAGE_PIN L21 [get_ports {pmod_gpio[6]}]
set_property PACKAGE_PIN M16 [get_ports {led[2]}]
set_property PACKAGE_PIN N22 [get_ports {flash_dq[3]}]
set_property PACKAGE_PIN N21 [get_ports {flash_dq[2]}]
set_property PACKAGE_PIN M22 [get_ports {flash_dq[1]}]
set_property PACKAGE_PIN M21 [get_ports {flash_dq[0]}]
set_property PACKAGE_PIN N19 [get_ports rgmii_tx_en]
set_property PACKAGE_PIN P21 [get_ports {eth_led_out[0]}]
set_property PACKAGE_PIN P22 [get_ports {eth_led_out[1]}]
set_property PACKAGE_PIN N20 [get_ports {rgmii_rxd[3]}]
set_property PACKAGE_PIN P20 [get_ports {rgmii_rxd[2]}]
set_property PACKAGE_PIN N17 [get_ports flash_cs_n]
set_property PACKAGE_PIN T21 [get_ports {rgmii_txd[0]}]
set_property PACKAGE_PIN T22 [get_ports {eth_led_in[0]}]
set_property PACKAGE_PIN V21 [get_ports rgmii_tx_clk]
set_property PACKAGE_PIN W22 [get_ports {ram_dq[4]}]
set_property PACKAGE_PIN U22 [get_ports {rgmii_txd[1]}]
set_property PACKAGE_PIN V22 [get_ports {rgmii_txd[3]}]
set_property PACKAGE_PIN W21 [get_ports ram_rst_n]
set_property PACKAGE_PIN Y21 [get_ports {ram_dq[3]}]
set_property PACKAGE_PIN U20 [get_ports eth_rst_n]
set_property PACKAGE_PIN V20 [get_ports {rgmii_txd[2]}]
set_property PACKAGE_PIN U18 [get_ports rgmii_rx_clk]
set_property PACKAGE_PIN U19 [get_ports eth_mdio]
set_property PACKAGE_PIN R18 [get_ports {rgmii_rxd[1]}]
set_property PACKAGE_PIN R19 [get_ports rgmii_rx_dv]
set_property PACKAGE_PIN R20 [get_ports {eth_led_in[1]}]
set_property PACKAGE_PIN T19 [get_ports {rgmii_rxd[0]}]
set_property PACKAGE_PIN T20 [get_ports eth_mdc]
set_property PACKAGE_PIN Y20 [get_ports {ram_dq[2]}]
set_property PACKAGE_PIN AA20 [get_ports {ram_dq[1]}]
set_property PACKAGE_PIN W18 [get_ports {ram_dq[6]}]
set_property PACKAGE_PIN Y19 [get_ports {ram_dq[7]}]
set_property PACKAGE_PIN AB21 [get_ports ram_cs_n]
set_property PACKAGE_PIN V18 [get_ports {ram_dq[5]}]
set_property PACKAGE_PIN V19 [get_ports {ram_dq[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {fmc_a_hi[8]}]
set_property PACKAGE_PIN AB19 [get_ports ram_clk_n]
set_property IOSTANDARD LVCMOS18 [get_ports ram_clk_n]
set_property IOSTANDARD LVCMOS18 [get_ports ram_clk_p]
set_property IOSTANDARD LVCMOS18 [get_ports ram_clk_psc_n]
set_property IOSTANDARD LVCMOS18 [get_ports ram_clk_psc_p]
set_property IOSTANDARD LVCMOS18 [get_ports ram_cs_n]
set_property IOSTANDARD LVCMOS33 [get_ports fmc_ne2]
set_property IOSTANDARD LVCMOS18 [get_ports ram_dqs]
set_property PACKAGE_PIN AA21 [get_ports ram_dqs]
set_property PACKAGE_PIN AB20 [get_ports ram_clk_p]
set_property PACKAGE_PIN Y22 [get_ports ram_clk_psc_n]
set_property PACKAGE_PIN AA22 [get_ports ram_clk_psc_p]

########################################################################################################################
# Clock constraints

create_clock -period 40.000 -name clk_25mhz -waveform {0.000 20.000} [get_ports clk_25mhz]

#125 MHz (8 ns period) FMC clock
create_clock -period 8.000 -name fmc_clk -waveform {0.000 4.000} [get_ports fmc_clk]

create_generated_clock -name clk_125mhz -source [get_pins clocks/pll/CLKIN1] -master_clock [get_clocks clk_25mhz] [get_pins clocks/pll/CLKOUT0]
create_generated_clock -name clk_250mhz -source [get_pins clocks/pll/CLKIN1] -master_clock [get_clocks clk_25mhz] [get_pins clocks/pll/CLKOUT1]

######################################################
# CDC constraints

# Synchronizer max delays
set_max_delay -datapath_only -from [get_cells -hierarchical -filter { NAME =~  "*sync*" && NAME =~  "*a_ff*" }] -to [get_cells -hierarchical -filter { NAME =~  "*sync*" && NAME =~  "*reg_b*" }] 4.000
set_max_delay -from [get_cells -hierarchical -filter { NAME =~  "*sync*" && NAME =~  "*dout0_reg*" }] -to [get_cells -hierarchical -filter { NAME =~  "*sync*" && NAME =~  "*dout1_reg*" }] 4.000

# Async clock domains
set_clock_groups -asynchronous -group [get_clocks clk_125mhz] -group [get_clocks pclk_raw]
set_clock_groups -asynchronous -group [get_clocks clk_25mhz] -group [get_clocks pclk_raw]

######################################################
# Floorplanning

create_pblock pblock_crypt25519
add_cells_to_pblock [get_pblocks pblock_crypt25519] [get_cells -quiet [list apb1/crypt25519]]
resize_pblock [get_pblocks pblock_crypt25519] -add {CLOCKREGION_X0Y0:CLOCKREGION_X1Y0}

create_pblock pblock_apb6_3v3
add_cells_to_pblock [get_pblocks pblock_apb6_3v3] [get_cells -quiet [list apb6_flash_3v3]]
resize_pblock [get_pblocks pblock_apb6_3v3] -add {CLOCKREGION_X0Y3:CLOCKREGION_X0Y3}

create_pblock pblock_apb5_2v5
add_cells_to_pblock [get_pblocks pblock_apb5_2v5] [get_cells -quiet [list apb5_flash_2v5]]
resize_pblock [get_pblocks pblock_apb5_2v5] -add {CLOCKREGION_X1Y3:CLOCKREGION_X1Y3}

create_pblock pblock_apb4_1v8
add_cells_to_pblock [get_pblocks pblock_apb4_1v8] [get_cells -quiet [list apb4_flash_1v8]]
resize_pblock [get_pblocks pblock_apb4_1v8] -add {CLOCKREGION_X1Y2:CLOCKREGION_X1Y2}

create_pblock pblock_apb3_1v2
add_cells_to_pblock [get_pblocks pblock_apb3_1v2] [get_cells -quiet [list apb3_flash_1v2]]
resize_pblock [get_pblocks pblock_apb3_1v2] -add {CLOCKREGION_X1Y1:CLOCKREGION_X1Y1}

# debug ila
create_pblock pblock_mgmt
add_cells_to_pblock [get_pblocks pblock_mgmt] [get_cells -quiet [list apb1/ila apb2]]
resize_pblock [get_pblocks pblock_mgmt] -add {CLOCKREGION_X0Y1:CLOCKREGION_X1Y2}

######################################################
# Put the timestamp in the bitstream USERCODE

set_property BITSTREAM.CONFIG.USR_ACCESS TIMESTAMP [current_design]

######################################################
# Debug stuff

######################################################
# Bitstream generation flags

set_property BITSTREAM.CONFIG.CONFIGRATE 33 [current_design]
set_property CONFIG_VOLTAGE 1.8 [current_design]
set_property CFGBVS GND [current_design]
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]

set_property SLEW FAST [get_ports {flash_dq[3]}]
set_property SLEW FAST [get_ports {flash_dq[2]}]
set_property SLEW FAST [get_ports {flash_dq[1]}]
set_property SLEW FAST [get_ports {flash_dq[0]}]
set_property DRIVE 8 [get_ports {flash_dq[3]}]
set_property DRIVE 8 [get_ports {flash_dq[2]}]
set_property DRIVE 8 [get_ports {flash_dq[1]}]
set_property DRIVE 8 [get_ports {flash_dq[0]}]
set_property DRIVE 8 [get_ports flash_cs_n]
set_property SLEW FAST [get_ports flash_cs_n]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets fmc_apb\\.pclk]
