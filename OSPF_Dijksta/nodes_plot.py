from dash import Dash, dcc, html, Input, Output
import plotly
import sys
import os
import pandas as pd
import plotly.express as px
import datetime
import time
# import visdcc
import json
parent = os.path.dirname(sys.path[0])
# sys.path.append(parent)


json_file = ".practice\programs\subscription (8).json"
def read_json(json_file_path):
    with open(json_file_path) as json_file:
        data = json.load(json_file)
    return data
 

json_data = read_json(json_file)
json_data = json_data["messages"]
dataframe_values = list()
for packet in json_data:
    temp = dict()
    #time, mac, nodeid, packet, sensor_info
    time_value = datetime.datetime.fromtimestamp(packet["timestamp"] / 1e3)
    #time_value = time.strftime('%H:%M:%S', packet["timestamp"])
    temp["Time"] = str(time_value)[11:]
    temp["sensor_info"] = packet["payload"]["Sensor Info"]
    temp["packet"] = packet["payload"]["Packet ID"]
    temp["node_id"] = packet["payload"]["Board ID"]
    dataframe_values.append(temp)

df = pd.DataFrame.from_dict(dataframe_values) 
df.to_csv (r'values.csv', index = False, header=True)

csv_file = "values.csv"
df = pd.read_csv(csv_file)
df["Time"] = pd.to_datetime(df.Time).dt.time

app = Dash(__name__)

def slice_node_data(dataframe, node_id):
    return dataframe[dataframe["node_id"] == node_id]

node_2_data = slice_node_data(df, 2)
node_3_data = slice_node_data(df, 3)
node_4_data = slice_node_data(df, 4)
node_5_data = slice_node_data(df, 5)
node_6_data = slice_node_data(df, 6)
node_7_data = slice_node_data(df, 7)
node_8_data = slice_node_data(df, 8)
node_9_data = slice_node_data(df, 9)
node_10_data = slice_node_data(df, 10)
node_11_data = slice_node_data(df, 11)
node_12_data = slice_node_data(df, 12)
print(node_2_data)

app.layout = html.Div([
    html.H4('Sensor Plots'),
    dcc.Dropdown(
        id="dropdown",
        options=['sensor_info'],
        value='sensor_info'
    ),
    html.H4('Overall Plots', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graphall")),
    html.H4('Node 2 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph2")),
    html.H4('Node 3 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph3")),
    html.H4('Node 4 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph4")),
    html.H4('Node 5 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph5")),
    html.H4('Node 6 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph6")),
    html.H4('Node 7 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph7")),
    html.H4('Node 8 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph8")),
    html.H4('Node 9 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph9")),
    html.H4('Node 10 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph10")),
    html.H4('Node 11 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph11")),
    html.H4('Node 12 Plot.', style={'textAlign': 'center'}),
    html.Div(dcc.Graph(id="graph12")),
])


@app.callback(
    Output("graphall", "figure"),
    Output("graph2", "figure"),
    Output("graph3", "figure"),
    Output("graph4", "figure"),
    Output("graph5", "figure"),
    Output("graph6", "figure"),
    Output("graph7", "figure"),
    Output("graph8", "figure"),
    Output("graph9", "figure"),
    Output("graph10", "figure"),
    Output("graph11", "figure"),
    Output("graph12", "figure"), 
    Input("dropdown", "value"))
def update_bar_chart(dims):
    fig1 = px.scatter(df, x= df["Time"], y= df["sensor_info"], color= df["node_id"])
    #fig1.add_scatter(x= df["Time"], y=df["sensor_info"])

    fig2 = px.scatter(node_2_data, x= node_2_data["Time"], y= node_2_data["sensor_info"], color= node_2_data["node_id"],color_discrete_sequence=["blue"])
    fig2.add_scatter(x= node_2_data["Time"], y=node_2_data["sensor_info"])

    fig3 = px.scatter(node_3_data, x= node_3_data["Time"], y= node_3_data["sensor_info"], color= node_3_data["node_id"],color_discrete_sequence=["red"])
    fig3.add_scatter(x= node_3_data["Time"], y=node_3_data["sensor_info"])

    fig4 = px.scatter(node_4_data, x= node_4_data["Time"], y= node_4_data["sensor_info"], color= node_4_data["node_id"],color_discrete_sequence=["yellow"])
    fig4.add_scatter(x= node_4_data["Time"], y=node_4_data["sensor_info"])

    fig5 = px.scatter(node_5_data, x= node_5_data["Time"], y= node_5_data["sensor_info"], color= node_5_data["node_id"],color_discrete_sequence=["orange"])
    fig5.add_scatter(x= node_5_data["Time"], y=node_5_data["sensor_info"])

    fig6 = px.scatter(node_6_data, x= node_6_data["Time"], y= node_6_data["sensor_info"], color= node_6_data["node_id"],color_discrete_sequence=["green"])
    fig6.add_scatter(x= node_6_data["Time"], y=node_6_data["sensor_info"])

    fig7 = px.scatter(node_7_data, x= node_7_data["Time"], y= node_7_data["sensor_info"], color= node_7_data["node_id"],color_discrete_sequence=["blue"])
    fig7.add_scatter(x= node_7_data["Time"], y=node_7_data["sensor_info"])

    fig8 = px.scatter(node_8_data, x= node_8_data["Time"], y= node_8_data["sensor_info"], color= node_8_data["node_id"],color_discrete_sequence=["darkorange"])
    fig8.add_scatter(x= node_8_data["Time"], y=node_8_data["sensor_info"])

    fig9 = px.scatter(node_9_data, x= node_9_data["Time"], y= node_9_data["sensor_info"], color= node_9_data["node_id"],color_discrete_sequence=["lavender"])
    fig9.add_scatter(x= node_9_data["Time"], y=node_9_data["sensor_info"])

    fig10 = px.scatter(node_10_data, x= node_10_data["Time"], y= node_10_data["sensor_info"], color= node_10_data["node_id"],color_discrete_sequence=["indigo"])
    fig10.add_scatter(x= node_10_data["Time"], y=node_10_data["sensor_info"])

    fig11 = px.scatter(node_11_data, x= node_11_data["Time"], y= node_11_data["sensor_info"], color= node_11_data["node_id"],color_discrete_sequence=["blue"])
    fig11.add_scatter(x= node_11_data["Time"], y=node_11_data["sensor_info"])

    fig12 = px.scatter(node_12_data, x= node_12_data["Time"], y= node_12_data["sensor_info"], color= node_12_data["node_id"],color_discrete_sequence=["blue"])
    fig12.add_scatter(x= node_12_data["Time"], y=node_12_data["sensor_info"])
    print(df)
    return fig1, fig2, fig3, fig4, fig5, fig6, fig7, fig8, fig9, fig10, fig11, fig12
    # return fig1, fig2, fig3, fig7

app.run_server(debug=True)