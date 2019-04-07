#pragma once
#include <msclr/marshal_cppstd.h>
#include "mycommon.h"
#include "metro\MetroTexturesDatabase.h"
#include "MainForm.h"
#include "ui\TexturePropertiesViewer.h"
#include "ui\NodeSorter.h"

namespace MetroEX {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for TexturesDatabaseViewer
    /// </summary>
    public ref class TexturesDatabaseViewer : public System::Windows::Forms::Form
    {
    public:
        TexturesDatabaseViewer(MainForm^ form, MetroTexturesDatabase* data, System::Windows::Forms::ImageList^ imageList)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //

            mDataProvider = data;
            mMainForm = form;
            mOriginalRootNode = nullptr;
            mPropertiesViewer = nullptr;

            mFileExtensions = gcnew array<String^>(3);
            mFileExtensions[0] = ".2048";
            mFileExtensions[1] = ".1024";
            mFileExtensions[2] = ".512";

            this->dataTree->ImageList = imageList;

            this->FillWithData();
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~TexturesDatabaseViewer()
        {
            if (components)
            {
                delete components;
            }
        }

    protected:

    protected:

    protected:


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
    private: System::Windows::Forms::TextBox^  filterText;




    private: System::Windows::Forms::Timer^  filterTimer;
    private: System::Windows::Forms::TreeView^  dataTree;

    private: System::Windows::Forms::SplitContainer^  splitContainer1;
    private: System::Windows::Forms::PropertyGrid^  propertyGrid;

    private: System::ComponentModel::IContainer^  components;






    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>


#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->filterText = (gcnew System::Windows::Forms::TextBox());
            this->dataTree = (gcnew System::Windows::Forms::TreeView());
            this->filterTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
            this->propertyGrid = (gcnew System::Windows::Forms::PropertyGrid());
            this->tableLayoutPanel1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
            this->splitContainer1->Panel1->SuspendLayout();
            this->splitContainer1->Panel2->SuspendLayout();
            this->splitContainer1->SuspendLayout();
            this->SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this->tableLayoutPanel1->ColumnCount = 1;
            this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                50)));
            this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                50)));
            this->tableLayoutPanel1->Controls->Add(this->filterText, 0, 0);
            this->tableLayoutPanel1->Controls->Add(this->dataTree, 0, 1);
            this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
            this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
            this->tableLayoutPanel1->RowCount = 2;
            this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 4.811716F)));
            this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 95.18829F)));
            this->tableLayoutPanel1->Size = System::Drawing::Size(315, 901);
            this->tableLayoutPanel1->TabIndex = 0;
            // 
            // filterText
            // 
            this->filterText->Dock = System::Windows::Forms::DockStyle::Fill;
            this->filterText->Location = System::Drawing::Point(3, 3);
            this->filterText->Name = L"filterText";
            this->filterText->Size = System::Drawing::Size(309, 20);
            this->filterText->TabIndex = 0;
            this->filterText->TextChanged += gcnew System::EventHandler(this, &TexturesDatabaseViewer::filterText_TextChanged);
            // 
            // dataTree
            // 
            this->dataTree->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataTree->Location = System::Drawing::Point(3, 46);
            this->dataTree->Name = L"dataTree";
            this->dataTree->Size = System::Drawing::Size(309, 852);
            this->dataTree->TabIndex = 1;
            this->dataTree->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &TexturesDatabaseViewer::dataTree_NodeMouseClick);
            this->dataTree->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &TexturesDatabaseViewer::dataTree_NodeMouseDoubleClick);
            // 
            // filterTimer
            // 
            this->filterTimer->Interval = 1000;
            this->filterTimer->Tick += gcnew System::EventHandler(this, &TexturesDatabaseViewer::filterTimer_Tick);
            // 
            // splitContainer1
            // 
            this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer1->Location = System::Drawing::Point(0, 0);
            this->splitContainer1->Name = L"splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this->splitContainer1->Panel1->Controls->Add(this->tableLayoutPanel1);
            // 
            // splitContainer1.Panel2
            // 
            this->splitContainer1->Panel2->Controls->Add(this->propertyGrid);
            this->splitContainer1->Size = System::Drawing::Size(946, 901);
            this->splitContainer1->SplitterDistance = 315;
            this->splitContainer1->TabIndex = 1;
            // 
            // propertyGrid
            // 
            this->propertyGrid->Dock = System::Windows::Forms::DockStyle::Fill;
            this->propertyGrid->Location = System::Drawing::Point(0, 0);
            this->propertyGrid->Name = L"propertyGrid";
            this->propertyGrid->Size = System::Drawing::Size(627, 901);
            this->propertyGrid->TabIndex = 0;
            // 
            // TexturesDatabaseViewer
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(946, 901);
            this->Controls->Add(this->splitContainer1);
            this->Name = L"TexturesDatabaseViewer";
            this->Text = L"Textures Database Viewer";
            this->tableLayoutPanel1->ResumeLayout(false);
            this->tableLayoutPanel1->PerformLayout();
            this->splitContainer1->Panel1->ResumeLayout(false);
            this->splitContainer1->Panel2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
            this->splitContainer1->ResumeLayout(false);
            this->ResumeLayout(false);

        }

#pragma endregion

    private:
        void FillWithData();
        TreeNode^ mOriginalRootNode;
        MetroTexturesDatabase* mDataProvider;
        MainForm^ mMainForm;
        array<String^>^ mFileExtensions;
        TexturePropertiesViewer^ mPropertiesViewer;
        String^ GetRealPath(size_t index);
        void filterTimer_Tick(System::Object^ sender, System::EventArgs^ e);
        void filterText_TextChanged(System::Object^ sender, System::EventArgs^ e);
        void dataTree_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
        void FilterTreeView(TreeNode^ node, String^ text);
        TreeNode^ FindNode(TreeNode^ parent, String^ text);
        void SortNodesRecursively(TreeNode^ parent, NodeSorter^ sorter);
        void dataTree_NodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
};
}
