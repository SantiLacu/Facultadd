import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import os
import csv
import json
from datetime import datetime
import pandas as pd

class RegistroEmprendimientos:
    def __init__(self):
        self.base_path = r"C:\Users\Usuario\Documents\Proyectos\Registro de Empredimientos"
        self.config_file = os.path.join(self.base_path, "config.json")
        self.current_emprendimiento = None
        self.current_path = None
        
        # Crear directorio base si no existe
        os.makedirs(self.base_path, exist_ok=True)
        
        # Configuración de tema oscuro
        self.colors = {
            'bg': '#2b2b2b',
            'fg': '#ffffff',
            'select_bg': '#404040',
            'select_fg': '#ffffff',
            'button_bg': '#404040',
            'button_fg': '#ffffff',
            'entry_bg': '#404040',
            'entry_fg': '#ffffff',
            'frame_bg': '#363636'
        }
        
        self.setup_styles()
        self.mostrar_ventana_inicio()
    
    def setup_styles(self):
        """Configurar estilos para tema oscuro"""
        style = ttk.Style()
        style.theme_use('clam')
        
        # Configurar estilos para tema oscuro
        style.configure('Dark.TFrame', background=self.colors['bg'])
        style.configure('Dark.TLabel', background=self.colors['bg'], foreground=self.colors['fg'])
        style.configure('Dark.TButton', background=self.colors['button_bg'], foreground=self.colors['button_fg'])
        style.configure('Dark.TEntry', background=self.colors['entry_bg'], foreground=self.colors['entry_fg'])
        style.configure('Dark.TCombobox', background=self.colors['entry_bg'], foreground=self.colors['entry_fg'])
        style.configure('Dark.Treeview', background=self.colors['bg'], foreground=self.colors['fg'],
                       selectbackground=self.colors['select_bg'])
        style.configure('Dark.Treeview.Heading', background=self.colors['frame_bg'], foreground=self.colors['fg'])
    
    def cargar_config(self):
        """Cargar configuración del último emprendimiento"""
        try:
            if os.path.exists(self.config_file):
                with open(self.config_file, 'r') as f:
                    return json.load(f)
        except:
            pass
        return {}
    
    def guardar_config(self):
        """Guardar configuración del último emprendimiento"""
        config = {
            'ultimo_emprendimiento': self.current_emprendimiento
        }
        try:
            with open(self.config_file, 'w') as f:
                json.dump(config, f)
        except:
            pass
    
    def mostrar_ventana_inicio(self):
        """Mostrar ventana para elegir o crear emprendimiento"""
        self.root_inicio = tk.Tk()
        self.root_inicio.title("Registro de Emprendimientos - Inicio")
        self.root_inicio.geometry("500x400")
        self.root_inicio.configure(bg=self.colors['bg'])
        
        # Título
        titulo = tk.Label(self.root_inicio, text="Registro de Emprendimientos", 
                         font=("Arial", 18, "bold"), bg=self.colors['bg'], fg=self.colors['fg'])
        titulo.pack(pady=20)
        
        # Frame para emprendimientos existentes
        frame_existentes = tk.Frame(self.root_inicio, bg=self.colors['bg'])
        frame_existentes.pack(pady=10, padx=20, fill="both", expand=True)
        
        tk.Label(frame_existentes, text="Emprendimientos Existentes:", 
                font=("Arial", 12, "bold"), bg=self.colors['bg'], fg=self.colors['fg']).pack(anchor="w")
        
        # Listbox para emprendimientos
        self.lista_emprendimientos = tk.Listbox(frame_existentes, bg=self.colors['entry_bg'], 
                                               fg=self.colors['entry_fg'], selectbackground=self.colors['select_bg'])
        self.lista_emprendimientos.pack(fill="both", expand=True, pady=5)
        
        # Cargar emprendimientos existentes
        self.cargar_lista_emprendimientos()
        
        # Botones
        frame_botones = tk.Frame(self.root_inicio, bg=self.colors['bg'])
        frame_botones.pack(pady=10)
        
        btn_abrir = tk.Button(frame_botones, text="Abrir Seleccionado", 
                             command=self.abrir_emprendimiento_seleccionado,
                             bg=self.colors['button_bg'], fg=self.colors['button_fg'], 
                             font=("Arial", 10, "bold"))
        btn_abrir.pack(side="left", padx=5)
        
        btn_nuevo = tk.Button(frame_botones, text="Crear Nuevo", 
                             command=self.crear_nuevo_emprendimiento,
                             bg=self.colors['button_bg'], fg=self.colors['button_fg'], 
                             font=("Arial", 10, "bold"))
        btn_nuevo.pack(side="left", padx=5)
        
        # Cargar último emprendimiento automáticamente
        config = self.cargar_config()
        ultimo = config.get('ultimo_emprendimiento')
        if ultimo and os.path.exists(os.path.join(self.base_path, ultimo)):
            self.current_emprendimiento = ultimo
            self.root_inicio.after(100, self.abrir_emprendimiento)
        
        self.root_inicio.mainloop()
    
    def cargar_lista_emprendimientos(self):
        """Cargar lista de emprendimientos existentes"""
        self.lista_emprendimientos.delete(0, tk.END)
        try:
            for item in os.listdir(self.base_path):
                path = os.path.join(self.base_path, item)
                if os.path.isdir(path) and item != "__pycache__":
                    self.lista_emprendimientos.insert(tk.END, item)
        except:
            pass
    
    def abrir_emprendimiento_seleccionado(self):
        """Abrir el emprendimiento seleccionado de la lista"""
        selection = self.lista_emprendimientos.curselection()
        if selection:
            self.current_emprendimiento = self.lista_emprendimientos.get(selection[0])
            self.abrir_emprendimiento()
    
    def crear_nuevo_emprendimiento(self):
        """Crear un nuevo emprendimiento"""
        ventana_nuevo = tk.Toplevel(self.root_inicio)
        ventana_nuevo.title("Nuevo Emprendimiento")
        ventana_nuevo.geometry("400x200")
        ventana_nuevo.configure(bg=self.colors['bg'])
        
        tk.Label(ventana_nuevo, text="Nombre del Emprendimiento:", 
                font=("Arial", 12), bg=self.colors['bg'], fg=self.colors['fg']).pack(pady=10)
        
        entry_nombre = tk.Entry(ventana_nuevo, font=("Arial", 12), width=30,
                               bg=self.colors['entry_bg'], fg=self.colors['entry_fg'])
        entry_nombre.pack(pady=5)
        
        def crear():
            nombre = entry_nombre.get().strip()
            if nombre:
                path = os.path.join(self.base_path, nombre)
                if not os.path.exists(path):
                    os.makedirs(path)
                    self.current_emprendimiento = nombre
                    self.crear_archivos_base()
                    ventana_nuevo.destroy()
                    self.abrir_emprendimiento()
                else:
                    messagebox.showerror("Error", "Ya existe un emprendimiento con ese nombre")
            else:
                messagebox.showwarning("Advertencia", "Ingrese un nombre válido")
        
        btn_crear = tk.Button(ventana_nuevo, text="Crear", command=crear,
                             bg=self.colors['button_bg'], fg=self.colors['button_fg'], 
                             font=("Arial", 10, "bold"))
        btn_crear.pack(pady=10)
        
        entry_nombre.focus()
        entry_nombre.bind('<Return>', lambda e: crear())
    
    def crear_archivos_base(self):
        """Crear archivos CSV base para el emprendimiento"""
        self.current_path = os.path.join(self.base_path, self.current_emprendimiento)
        
        # Archivo de productos
        productos_file = os.path.join(self.current_path, "productos.csv")
        if not os.path.exists(productos_file):
            with open(productos_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                writer.writerow(['PRODUCTO', 'COLOR', 'TELA', 'TALLE', 'PRECIO_VENTA', 'STOCK_ACTUAL'])
        
        # Archivo de movimientos
        movimientos_file = os.path.join(self.current_path, "movimientos.csv")
        if not os.path.exists(movimientos_file):
            with open(movimientos_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                writer.writerow(['FECHA', 'TIPO', 'PRODUCTO', 'COLOR', 'TELA', 'TALLE', 
                               'CANTIDAD', 'PRECIO', 'CLIENTE_PROVEEDOR', 'COSTO', 'GANANCIA'])
        
        # Archivo de compras
        compras_file = os.path.join(self.current_path, "compras.csv")
        if not os.path.exists(compras_file):
            with open(compras_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                writer.writerow(['FECHA', 'PRODUCTO', 'COLOR', 'TELA', 'TALLE', 'CANTIDAD', 
                               'PRECIO', 'COSTO', 'PROVEEDOR', 'INVERSION'])
        
        # Archivo de ventas
        ventas_file = os.path.join(self.current_path, "ventas.csv")
        if not os.path.exists(ventas_file):
            with open(ventas_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                writer.writerow(['FECHA', 'PRODUCTO', 'COLOR', 'TELA', 'TALLE', 'CANTIDAD', 
                               'PRECIO', 'COSTO', 'GANANCIA', 'CLIENTE'])
    
    def abrir_emprendimiento(self):
        """Abrir la ventana principal del emprendimiento"""
        self.current_path = os.path.join(self.base_path, self.current_emprendimiento)
        self.crear_archivos_base()
        self.guardar_config()
        
        # Cerrar ventana de inicio
        if hasattr(self, 'root_inicio'):
            self.root_inicio.destroy()
        
        # Crear ventana principal
        self.root = tk.Tk()
        self.root.title(f"Registro de Emprendimientos - {self.current_emprendimiento}")
        self.root.geometry("800x600")
        self.root.configure(bg=self.colors['bg'])
        
        # Título principal
        titulo = tk.Label(self.root, text=f"Gestión de {self.current_emprendimiento}", 
                         font=("Arial", 18, "bold"), bg=self.colors['bg'], fg=self.colors['fg'])
        titulo.pack(pady=20)
        
        # Frame principal para botones
        frame_botones = tk.Frame(self.root, bg=self.colors['bg'])
        frame_botones.pack(pady=20)
        
        # Botones principales
        botones = [
            ("Agregar Producto", self.agregar_producto),
            ("Registrar Compra", self.registrar_compra),
            ("Registrar Venta", self.registrar_venta),
            ("Retirar Stock", self.retirar_stock),
            ("Ver Stock Actual", self.ver_stock),
            ("Exportar Stock", self.exportar_stock)
        ]
        
        for i, (texto, comando) in enumerate(botones):
            btn = tk.Button(frame_botones, text=texto, command=comando,
                           bg=self.colors['button_bg'], fg=self.colors['button_fg'], 
                           font=("Arial", 12, "bold"), width=20, height=2)
            btn.grid(row=i//2, column=i%2, padx=10, pady=10)
        
        # Frame para mostrar información
        self.frame_info = tk.Frame(self.root, bg=self.colors['bg'])
        self.frame_info.pack(fill="both", expand=True, padx=20, pady=10)
        
        # Botón para cambiar emprendimiento
        btn_cambiar = tk.Button(self.root, text="Cambiar Emprendimiento", 
                               command=self.cambiar_emprendimiento,
                               bg=self.colors['frame_bg'], fg=self.colors['fg'], 
                               font=("Arial", 10))
        btn_cambiar.pack(pady=10)
        
        self.cargar_datos_emprendimiento()
        self.root.mainloop()
    
    def cambiar_emprendimiento(self):
        """Volver a la pantalla de selección de emprendimiento"""
        self.root.destroy()
        self.mostrar_ventana_inicio()
    
    def cargar_datos_emprendimiento(self):
        """Cargar datos iniciales del emprendimiento"""
        # Limpiar frame de información
        for widget in self.frame_info.winfo_children():
            widget.destroy()
        
        # Mostrar resumen del emprendimiento
        label_resumen = tk.Label(self.frame_info, 
                                text="Seleccione una acción del menú superior para comenzar",
                                font=("Arial", 14), bg=self.colors['bg'], fg=self.colors['fg'])
        label_resumen.pack(pady=50)
    
    def limpiar_frame_info(self):
        """Limpiar el frame de información"""
        for widget in self.frame_info.winfo_children():
            widget.destroy()
    
    def agregar_producto(self):
        """Función para agregar un nuevo producto"""
        self.limpiar_frame_info()
        
        # Título de la sección
        titulo = tk.Label(self.frame_info, text="Agregar Nuevo Producto", 
                         font=("Arial", 16, "bold"), bg=self.colors['bg'], fg=self.colors['fg'])
        titulo.pack(pady=10)
        
        # Mensaje de ayuda
        ayuda = tk.Label(self.frame_info, 
                        text="Complete todos los campos. Los campos se autocompletarán con productos existentes.",
                        font=("Arial", 10), bg=self.colors['bg'], fg=self.colors['fg'])
        ayuda.pack(pady=5)
        
        # Frame para el formulario
        form_frame = tk.Frame(self.frame_info, bg=self.colors['bg'])
        form_frame.pack(pady=20)
        
        # Variables para los campos
        self.var_nombre = tk.StringVar()
        self.var_color = tk.StringVar()
        self.var_tela = tk.StringVar()
        self.var_talle = tk.StringVar()
        self.var_precio = tk.StringVar()
        
        # Campos del formulario
        campos = [
            ("Nombre del Producto:", self.var_nombre),
            ("Color:", self.var_color),
            ("Tela:", self.var_tela),
            ("Talle:", self.var_talle),
            ("Precio de Venta:", self.var_precio)
        ]
        
        self.entries = {}
        for i, (label_text, var) in enumerate(campos):
            tk.Label(form_frame, text=label_text, font=("Arial", 11), 
                    bg=self.colors['bg'], fg=self.colors['fg']).grid(row=i, column=0, sticky="w", pady=5)
            
            entry = tk.Entry(form_frame, textvariable=var, font=("Arial", 11), width=30,
                           bg=self.colors['entry_bg'], fg=self.colors['entry_fg'])
            entry.grid(row=i, column=1, padx=10, pady=5)
            self.entries[label_text] = entry
        
        # Botón para agregar
        btn_agregar = tk.Button(form_frame, text="Agregar Producto", 
                               command=self.procesar_agregar_producto,
                               bg=self.colors['button_bg'], fg=self.colors['button_fg'], 
                               font=("Arial", 12, "bold"))
        btn_agregar.grid(row=len(campos), column=0, columnspan=2, pady=20)
        
        # Frame para mostrar productos recientes
        self.frame_recientes = tk.Frame(self.frame_info, bg=self.colors['bg'])
        self.frame_recientes.pack(fill="both", expand=True, pady=10)
        
        self.mostrar_productos_recientes()
    
    def procesar_agregar_producto(self):
        """Procesar la adición de un nuevo producto"""
        # Obtener valores
        nombre = self.var_nombre.get().strip()
        color = self.var_color.get().strip()
        tela = self.var_tela.get().strip()
        talle = self.var_talle.get().strip()
        precio = self.var_precio.get().strip()
        
        # Validar campos
        if not all([nombre, color, tela, talle, precio]):
            messagebox.showwarning("Campos Vacíos", "Por favor, complete todos los campos")
            return
        
        try:
            precio_float = float(precio)
        except ValueError:
            messagebox.showerror("Error", "El precio debe ser un número válido")
            return
        
        # Agregar al archivo de productos
        productos_file = os.path.join(self.current_path, "productos.csv")
        with open(productos_file, 'a', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow([nombre, color, tela, talle, precio_float, 0])  # Stock inicial 0
        
        # Limpiar campos
        for var in [self.var_nombre, self.var_color, self.var_tela, self.var_talle, self.var_precio]:
            var.set("")
        
        messagebox.showinfo("Éxito", f"Producto '{nombre}' agregado correctamente")
        self.mostrar_productos_recientes()
    
    def mostrar_productos_recientes(self):
        """Mostrar los últimos productos agregados"""
        # Limpiar frame de recientes
        for widget in self.frame_recientes.winfo_children():
            widget.destroy()
        
        tk.Label(self.frame_recientes, text="Últimos Productos Agregados:", 
                font=("Arial", 12, "bold"), bg=self.colors['bg'], fg=self.colors['fg']).pack(anchor="w")
        
        # Crear tabla para mostrar productos
        tree = ttk.Treeview(self.frame_recientes, style="Dark.Treeview")
        tree['columns'] = ('Producto', 'Color', 'Tela', 'Talle', 'Precio')
        tree['show'] = 'headings'
        
        for col in tree['columns']:
            tree.heading(col, text=col)
            tree.column(col, width=120)
        
        # Cargar últimos 5 productos
        try:
            productos_file = os.path.join(self.current_path, "productos.csv")
            if os.path.exists(productos_file):
                with open(productos_file, 'r', encoding='utf-8') as f:
                    reader = csv.reader(f)
                    productos = list(reader)[1:]  # Saltar encabezados
                    
                    # Mostrar últimos 5
                    for producto in productos[-5:]:
                        if len(producto) >= 5:
                            tree.insert('', 'end', values=producto[:5])
        except Exception as e:
            pass
        
        tree.pack(fill="both", expand=True, pady=5)
    
    def registrar_compra(self):
        """Función para registrar una compra - placeholder"""
        messagebox.showinfo("En desarrollo", "Función de registrar compra próximamente")
    
    def registrar_venta(self):
        """Función para registrar una venta - placeholder"""
        messagebox.showinfo("En desarrollo", "Función de registrar venta próximamente")
    
    def retirar_stock(self):
        """Función para retirar stock - placeholder"""
        messagebox.showinfo("En desarrollo", "Función de retirar stock próximamente")
    
    def ver_stock(self):
        """Función para ver stock actual - placeholder"""
        messagebox.showinfo("En desarrollo", "Función de ver stock próximamente")
    
    def exportar_stock(self):
        """Función para exportar stock - placeholder"""
        messagebox.showinfo("En desarrollo", "Función de exportar stock próximamente")

if __name__ == "__main__":
    app = RegistroEmprendimientos()