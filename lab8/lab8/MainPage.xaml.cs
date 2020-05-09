using System;
using System.Collections.Generic;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;

namespace lab7
{

    public sealed partial class MainPage : Page
    {
        private StorageFolder currentFolder;
        public MainPage()
        {
            this.InitializeComponent();
        }
        
        private async void ChooseStartFolderButton_Click(object sender, RoutedEventArgs e)
        {
            Windows.Storage.Pickers.FolderPicker folderPicker = new Windows.Storage.Pickers.FolderPicker();
            folderPicker.ViewMode = Windows.Storage.Pickers.PickerViewMode.Thumbnail;
            folderPicker.FileTypeFilter.Add(".txt");

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();
            ScanDirectory(folder);
        }

        private async void ScanDirectory(StorageFolder folder)
        {
            if (folder != null)
            {
                listBox.Items.Clear();
                currentFolder = folder;
                StorageFolder parentFolder = await folder.GetParentAsync();
                ListBoxItem listBoxItem = new ListBoxItem();
                listBoxItem.Content = "Back";
                listBoxItem.DoubleTapped += Navigate;
                listBoxItem.Tag = parentFolder;
                listBox.Items.Add(listBoxItem);

                IReadOnlyList<IStorageItem> itemsList = await folder.GetItemsAsync();
                foreach (var item in itemsList)
                {
                    listBoxItem = new ListBoxItem();
                    listBoxItem.Tag = item;
                    if (item is StorageFolder)
                    {
                        listBoxItem.Content = "[Folder]" + item.Name + "\tCreated: " + item.DateCreated.DateTime;
                    }
                    else
                    {
                        BasicProperties properties = await item.GetBasicPropertiesAsync();
                        listBoxItem.Content = "[Folder]" + item.Name + "\tCreated: " + item.DateCreated.DateTime + "\tSize: " + GetSize(properties.Size);
                    }
                    listBox.Items.Add(listBoxItem);
                    listBoxItem.DoubleTapped += Navigate;
                }
            }
        }

        private string GetSize(ulong byteSize)
        {
            ulong finalSize = byteSize;
            string endString = " B";
            if (finalSize > 1024)
            {
                endString = " kB";
                finalSize /= 1024;
            }

            if (finalSize > 1024)
            {
                endString = " MB";
                finalSize /= 1024;
            }

            if (finalSize > 1024)
            {
                endString = " GB";
                finalSize /= 1024;
            }
            return finalSize+endString;
        }

        private void Navigate(object sender, DoubleTappedRoutedEventArgs e)
        {
            ListBoxItem listBoxItem = (ListBoxItem)sender;
            if (listBoxItem.Tag is StorageFolder)
            {
                ScanDirectory((StorageFolder)listBoxItem.Tag);
            }
        }

        private async void DeleteFolderButton_Click(object sender, RoutedEventArgs e)
        {
            if (listBox.SelectedItem == null)
                return;

            ListBoxItem listBoxItem = (ListBoxItem)listBox.SelectedItem;
            if (listBoxItem.Tag is StorageFolder)
            {
                StorageFolder folder = (StorageFolder)listBoxItem.Tag;
                await folder.DeleteAsync();
            }
            ScanDirectory(currentFolder);
        }

        private async void CreateFolderButton_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(folderName.Text) && !string.IsNullOrEmpty(currentFolder.Name))
            {
                await currentFolder.CreateFolderAsync(folderName.Text);
                ScanDirectory(currentFolder);
            }
        }
    }
}
