bootblock-y += bootblock.c

ramstage-$(CONFIG_GENERATE_PIRQ_TABLE) += irq_tables.c
ramstage-$(CONFIG_GENERATE_MP_TABLE) += mptable.c
