from tkinter import *
import tkinter as tk
from tkinter import ttk
import serial
import threading
import mysql.connector

buf = bytearray(50)

connection = mysql.connector.connect(
    host="127.0.0.1",
    user="root",
    password="Iamcrowley666",
    database="kainska"
)
cursor = connection.cursor()

select_settings = "select * from settings"
cursor.execute(select_settings)
results = cursor.fetchall()
(id, comport, speed) = results[0]

ser = serial.Serial(comport, speed, timeout=1)


def update_table():
    # get log's from db
    select_logs = "select time as data, log from logdata"
    cursor.execute(select_logs)
    results = cursor.fetchall()

    # clean table
    for i in tree.get_children():
        tree.delete(i)

    # add new logs to the table
    for row in results:
        tree.insert("", tk.END, values=row)


def get_log():
    data = ""
    while True:
        ser.readinto(buf)
        if (buf[0] == 13):
            print("Got: ", data)
            insert_log = "insert into logdata (log) values ('" + data + "') "
            cursor.execute(insert_log)
            connection.commit()
            update_table()
            data = ""
            buf[0] = 0
        else:
            if buf[0] >= 32:
                data += chr(buf[0])


def get_pass_from_app():
    input_pass1 = T_user_1.get("1.0", "end-1c")
    input_pass2 = T_user_2.get("1.0", "end-1c")
    input_pass3 = T_user_3.get("1.0", "end-1c")
    input_pass4 = T_user_4.get("1.0", "end-1c")

    passwords = input_pass1 + "^" + input_pass2 + "^" + input_pass3 + "^" + input_pass4 + "^"

    print(passwords)
    passwords = passwords.encode()
    byte_array = bytearray(passwords)

    ser.write(byte_array)
    print(passwords)
    T_user_1.delete("1.0", "end")
    T_user_2.delete("1.0", "end")
    T_user_3.delete("1.0", "end")
    T_user_4.delete("1.0", "end")


root = tk.Tk()
root.geometry("560x600")
root.title("Anastasiia Kainska")

L_user_1 = Label(root, text="User #1", fg="#eee", bg="#333", font="14")
L_user_1.place(x=90, y=15)
T_user_1 = Text(root, height=3, width=30)
T_user_1.place(x=10, y=50)

L_user_2 = Label(root, text="User #2", fg="#eee", bg="#333", font="14")
L_user_2.place(x=385, y=15)
T_user_2 = Text(root, height=3, width=30)
T_user_2.place(x=300, y=50)

L_user_3 = Label(root, text="User #3", fg="#eee", bg="#333", font="14")
L_user_3.place(x=90, y=115)
T_user_3 = Text(root, height=3, width=30)
T_user_3.place(x=10, y=150)

L_user_4 = Label(root, text="User #4", fg="#eee", bg="#333", font="14")
L_user_4.place(x=385, y=115)
T_user_4 = Text(root, height=3, width=30)
T_user_4.place(x=300, y=150)

Button_send_pass = Button(root, text="Send", padx=10, pady=13, command=get_pass_from_app)
Button_send_pass.place(x=250, y=100)

# table creation
tree = ttk.Treeview(root, column=("c1", "c2"), show='headings')

tree.column("#1", anchor=tk.CENTER)
tree.heading("#1", text="Date")
tree.column("#2", anchor=tk.CENTER)
tree.heading("#2", text="Event")
tree.place(x=80, y=250)

# new thread for geting data from serial port
t = threading.Thread(target=get_log, daemon=True)
t.start()

root.mainloop()