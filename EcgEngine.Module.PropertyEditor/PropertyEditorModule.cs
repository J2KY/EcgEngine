﻿using EcgEngine.Module.PropertyEditor.ViewModels;
using EcgEngine.Module.PropertyEditor.Views;
using Prism.Ioc;
using Prism.Modularity;

namespace EcgEngine.Module.PropertyEditor
{
    public class PropertyEditorModule : IModule
    {
        public void OnInitialized(IContainerProvider containerProvider)
        {

        }

        public void RegisterTypes(IContainerRegistry containerRegistry)
        {
            containerRegistry.RegisterForNavigation<PropertyEditorView, PropertyEditorViewModel>();
        }
    }
}