import tkinter as tk

def calcular(operador):
    try:
        num1 = float(entrada1.get())
        num2 = float(entrada2.get())

        if operador == '+':
            resultado.set(num1 + num2)

        elif operador == '-':
            resultado.set(num1 - num2)

        elif operador == '*':
            resultado.set(num1 * num2)

        elif operador == '/':
            if num2 != 0:
                resultado.set(num1 / num2)
            else:
                resultado.set("Error: División entre 0")

    except ValueError:
        resultado.set("Error: Entrada inválida")

# Crear ventana principal
ventana = tk.Tk()
ventana.title("Calculadora")
ventana.geometry("400x400")

# Variables
entrada1 = tk.Entry(ventana, font=("Arial", 16))
entrada2 = tk.Entry(ventana, font=("Arial", 16))
resultado = tk.StringVar()

# Widgets
tk.Label(ventana, text="Número 1:", font=("Arial", 14)).pack(pady=5)
entrada1.pack(pady=5)

tk.Label(ventana, text="Número 2:", font=("Arial", 14)).pack(pady=5)
entrada2.pack(pady=5)

tk.Button(ventana, text="+", font=("Arial", 16), width=5, height=2, command=lambda: calcular('+')).pack(pady=5)
tk.Button(ventana, text="-", font=("Arial", 16), width=5, height=2, command=lambda: calcular('-')).pack(pady=5)
tk.Button(ventana, text="*", font=("Arial", 16), width=5, height=2, command=lambda: calcular('*')).pack(pady=5)
tk.Button(ventana, text="/", font=("Arial", 16), width=5, height=2, command=lambda: calcular('/')).pack(pady=5)

tk.Label(ventana, text="Resultado:", font=("Arial", 14)).pack(pady=5)
tk.Label(ventana, textvariable=resultado, font=("Arial", 18), fg="blue").pack(pady=10)

# Iniciar la aplicación
ventana.mainloop()