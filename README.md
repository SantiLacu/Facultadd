# Registro de Emprendimientos

Sistema de gestión de inventario para emprendimientos con interfaz gráfica en Python.

## Características Implementadas ✅

### **Funcionalidades Principales**
- ✅ **Multi-emprendimiento**: Cada emprendimiento tiene su carpeta separada
- ✅ **Ventana de inicio**: Crear o seleccionar emprendimientos existentes
- ✅ **Tema oscuro**: Interfaz completa con colores oscuros
- ✅ **Agregar Producto**: Funcionalidad completa implementada
- ✅ **Gestión de archivos CSV**: Creación automática de archivos estructurados
- ✅ **Configuración persistente**: Recuerda el último emprendimiento abierto

### **Funcionalidad "Agregar Producto"**
- Formulario con campos individuales (Nombre, Color, Tela, Talle, Precio)
- Mensaje de ayuda siempre visible
- Validación completa de campos
- Muestra últimos productos agregados en tabla
- Guardado automático en CSV

### **Estructura de Archivos**
Cada emprendimiento genera automáticamente:
- `productos.csv` - Catálogo de productos con stock
- `movimientos.csv` - Historial de todos los movimientos
- `compras.csv` - Registro de compras
- `ventas.csv` - Registro de ventas

## Instalación y Uso

### **Requisitos**
- Python 3.7 o superior
- tkinter (incluido en la mayoría de instalaciones de Python)
- pandas (se instala automáticamente si no está disponible)

### **Para Windows**
1. **Instalar Python** (si no lo tienes):
   - Descargar desde [python.org](https://www.python.org/downloads/)
   - Durante la instalación, marcar "Add Python to PATH"

2. **Ejecutar el programa**:
   ```bash
   python registro_emprendimientos.py
   ```

3. **Ubicación de datos**:
   Los archivos se guardan en: `C:\Users\Usuario\Documents\Proyectos\Registro de Empredimientos`

### **Primera vez**
1. Al abrir, aparece ventana para crear o seleccionar emprendimiento
2. Click en "Crear Nuevo" 
3. Ingresa el nombre (ej: "JS Indumentaria")
4. Se abre la ventana principal con 6 botones de acción

## Próximas Funcionalidades

### **En Desarrollo**
- 🔄 **Registrar Compra**: Con lista desplegable y stock actualizado
- 🔄 **Registrar Venta**: Igual que compra
- 🔄 **Retirar Stock**: Con búsqueda dinámica y confirmación
- 🔄 **Ver Stock**: Tabla con filtros avanzados
- 🔄 **Exportar Stock**: Generación de CSV con fecha

### **Flujo Actual**
1. **Inicio**: Seleccionar/crear emprendimiento
2. **Agregar Producto**: ✅ Completamente funcional
3. **Otras acciones**: Mostrarán mensaje "En desarrollo"

## Estructura del Proyecto

```
C:\Users\Usuario\Documents\Proyectos\Registro de Empredimientos\
├── config.json                    # Configuración global
├── [Nombre_Emprendimiento_1]/
│   ├── productos.csv
│   ├── movimientos.csv
│   ├── compras.csv
│   └── ventas.csv
├── [Nombre_Emprendimiento_2]/
│   ├── productos.csv
│   ├── movimientos.csv
│   ├── compras.csv
│   └── ventas.csv
└── ...
```

## Soporte

El programa está siendo desarrollado iterativamente. Si encuentras algún problema o quieres solicitar nuevas funcionalidades, el desarrollo continuará función por función según tus necesidades.

**Estado actual**: Base sólida con gestión de productos completamente funcional.
