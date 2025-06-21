import yfinance as yf
# import yfinance_cache as yf
import streamlit as st
import pandas as pd
import plotly.express as px

st.set_page_config(layout="wide")


# We will use Amazon stocks
# stock = 'MMM'
#
# # Get stock data
# get_stock_data = yf.Ticker(stock)
#
# # Set the time line of your data
# ticket_df = get_stock_data.history(period='1h', start='2000-1-02', end='2023-12-12')
#
# div = get_stock_data.dividends
#
#
# # Show your data in line chart
# st.line_chart(ticket_df.Close)
# st.line_chart(ticket_df.Volume)
# st.line_chart(div)
#
# get_stock_data.info['longBusinessSummary']
# div
filename = 'myportfolio.txt'
try:
    with open(filename) as f:
        tickers = set(f.read().splitlines())
except (FileNotFoundError, IndexError):
    ticker = 'AAPL'


def add():
    tickers.add(selected_stock)
    print(selected_stock)
    with open(filename, 'w') as f:
        f.writelines(line + '\n' for line in tickers)


def update():
    stock_data = yf.Ticker(rd_tickers)
    info = stock_data.info

    col1, col2 = st.columns([6, 1])
    col1.write(f'# {info["longName"]}')

    logo_url = info.get('logo_url', '')
    try:
        if logo_url != '':
            col2.image(logo_url, width=70)
    except TypeError:
        pass

    col1, col2, col3 = st.columns([1, 1, 5])
    col1.write(f'Ticker: {info.get("symbol", "")}')
    col2.write(f'Exchange: {info.get("exchange", "")}')
    col3.write(f'Currency: {info.get("financialCurrency", "")}')
    with st.expander("Company summary"):
        descr = stock_data.info['longBusinessSummary']
        st.write(descr)
        web = info.get('website', '')
        st.write(f'[{web}]({web})')
        st.write("Info method from yfinance")
        info


    col1, col2, col3 = st.columns(3)

    col1.write(f'Sector: {info.get("sector", "")}')
    col2.write(f'Industry: {info.get("industry", "")}')

    div = stock_data.dividends
    fig = px.scatter(div, color_discrete_sequence=['yellow'])
    fig.update_xaxes(rangeslider_visible=True)
    #st.plotly_chart(fig)

    col1, col2 = st.columns(2)
    fin = stock_data.financials.T.sort_index()
    fin['date'] = fin.index.values
    fin['date'] = fin['date'].astype(str)
    fin['period'] = 0
    fin1 = stock_data.quarterly_financials.T.sort_index()
    fin1['date'] = fin1.index.values
    fin1['date'] = fin1['date'].astype(str)
    fin1['period'] = 1

    # fin = pd.concat([fin, fin1])

    fig = px.bar(fin,  y='Net Income', x='date',
                 color_discrete_sequence=['green'],
                 title='Net Income, annual', width=500, height=350)# text='Net Income')
    fig.update_xaxes(type='category')
    # fig.update_layout(xaxis=dict(tickformat="%Y-%m-%d %H"))
    # fig.update_traces(texttemplate=div['Net Income'], textposition='outside')
    col1.plotly_chart(fig)

    fig = px.bar(fin1,  y='Net Income', x='date',
                 color_discrete_sequence=['green'],
                 title='Net Income, quater', width=500, height=350)# text='Net Income')
    fig.update_xaxes(type='category')
    col2.plotly_chart(fig)

    fig = px.bar(fin,  y='Total Revenue', x='date',
                 color_discrete_sequence=['yellow'], width=500, height=350)
    fig.update_xaxes(type='category')
    col1.plotly_chart(fig)

    fig = px.bar(fin1,  y='Total Revenue', x='date',
                 color_discrete_sequence=['yellow'], width=500, height=350)
    fig.update_xaxes(type='category')
    col2.plotly_chart(fig)


    #st.write(div)

    #st.write(stock_data.shares)

    #st.line_chart(div)


# if __name__ == "__main__":
#     go()


st.sidebar.subheader("""my portfolio analysis""")
selected_stock = st.sidebar.text_input("Add ticker to list", '')
button_clicked = st.sidebar.button("ADD")

if button_clicked:
    add()

rd_tickers = st.sidebar.radio(
    "Choose ticker",
    tickers,
    help=f"tickers from file {filename}",
)


if rd_tickers:
    update()
