import streamlit as st

st.set_page_config(layout="wide")

col1, col2, col3, col4 = st.columns([1, 1, 1, 4])

usd_rub = col1.text_input('USD/RUB exchange rate', '75.90')
usd_rub = float(usd_rub)

rus_com = col2.text_input('Commission for rub/usd exchange, %', '3')
rus_com = float(rus_com)

bank_rus = st.text_input('Raiffeisen bank comission, USB', '300')
bank_rus = int(bank_rus)

nis_usd = col1.text_input('NIS/USD on exchange', '3.60')
nis_usd = float(nis_usd)

isr_com = col2.text_input('Israeli bank commission, usd', '170')

isr_cur = col1.text_input('Isr bank commission for NIS/USD exch', '1')
isr_cur = float(isr_cur)
isr_fix = col2.text_input('Israeli bank fixed, nis', '170')
isr_fix = int(isr_fix)


flat_price = col1.text_input('My flat price on AVITO, RUB', '6600000')
flat_price = int(flat_price)

r_com = col2.text_input('Agency commission, RUB', '0')
r_com = int(r_com)


c0 = flat_price - r_com
c2 = c0 / (1 + rus_com/100)
c1 = c2 / 100 * rus_com

c3 = c2 / usd_rub
c4 = c3 - bank_rus - isr_fix
c5 = nis_usd + nis_usd / 100 * isr_cur
c6 = c4 * c5

# c7 = c6 - isr_fix

col3.write(f'Price for sq.m {flat_price/57.5:,.0f} RUB')

col3.write(f'I get after agency {c0:,} RUB')

col3.write(f'Russian exchange commission {c1:,.0f} RUB')

col3.write(f'Sum for buying USD on stock exch {c2:,.0f} RUB')

col3.write(f'I get after buying USD on stock exch {c3:,.0f} USD')

col3.write(f'I get on israeli bank {c4:,.0f} USD')

col3.write(f'Exchange rate: {nis_usd},   isr bank rate  {c5}')

col3.write(f'I get on my isr account after conversion {c6:,.0f} NIS')

# col3.write(f'After paying fixed sum {c7:,.0f} NIS')




