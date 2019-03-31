#pragma once
#include "mycommon.h"
#include "metro\MetroTexturesDatabase.h"
#include "MainForm.h"

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
        TexturesDatabaseViewer(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //

            mDataProvider = nullptr;

            mFileExtensions = gcnew array<String^>(3);
            mFileExtensions[0] = ".2048";
            mFileExtensions[1] = ".1024";
            mFileExtensions[2] = ".512";
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
    private: System::Data::DataSet^  texturesDataSet;
    protected:

    protected:

    protected:


    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
    private: System::Windows::Forms::TextBox^  filterText;

    private: System::Windows::Forms::DataGridView^  dataGridView;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  textureName;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  textureStreamable;
    private: System::Windows::Forms::Timer^  filterTimer;
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
            this->texturesDataSet = (gcnew System::Data::DataSet());
            this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->filterText = (gcnew System::Windows::Forms::TextBox());
            this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->textureName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->textureStreamable = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->filterTimer = (gcnew System::Windows::Forms::Timer(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->texturesDataSet))->BeginInit();
            this->tableLayoutPanel1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->BeginInit();
            this->SuspendLayout();
            // 
            // texturesDataSet
            // 
            this->texturesDataSet->DataSetName = L"TexturesDataSet";
            this->texturesDataSet->Locale = (gcnew System::Globalization::CultureInfo(L"en-US"));
            // 
            // tableLayoutPanel1
            // 
            this->tableLayoutPanel1->ColumnCount = 1;
            this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                50)));
            this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                50)));
            this->tableLayoutPanel1->Controls->Add(this->filterText, 0, 0);
            this->tableLayoutPanel1->Controls->Add(this->dataGridView, 0, 1);
            this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
            this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
            this->tableLayoutPanel1->RowCount = 2;
            this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 4.811716F)));
            this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 95.18829F)));
            this->tableLayoutPanel1->Size = System::Drawing::Size(649, 478);
            this->tableLayoutPanel1->TabIndex = 0;
            // 
            // filterText
            // 
            this->filterText->Dock = System::Windows::Forms::DockStyle::Fill;
            this->filterText->Location = System::Drawing::Point(3, 3);
            this->filterText->Name = L"filterText";
            this->filterText->Size = System::Drawing::Size(643, 20);
            this->filterText->TabIndex = 0;
            this->filterText->TextChanged += gcnew System::EventHandler(this, &TexturesDatabaseViewer::filterText_TextChanged);
            // 
            // dataGridView
            // 
            this->dataGridView->AllowUserToAddRows = false;
            this->dataGridView->AllowUserToDeleteRows = false;
            this->dataGridView->AllowUserToResizeRows = false;
            this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {
                this->textureName,
                    this->textureStreamable
            });
            this->dataGridView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->dataGridView->Location = System::Drawing::Point(3, 26);
            this->dataGridView->MultiSelect = false;
            this->dataGridView->Name = L"dataGridView";
            this->dataGridView->Size = System::Drawing::Size(643, 449);
            this->dataGridView->TabIndex = 1;
            this->dataGridView->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &TexturesDatabaseViewer::dataGridView_MouseDoubleClick);
            // 
            // textureName
            // 
            this->textureName->HeaderText = L"Name";
            this->textureName->Name = L"textureName";
            this->textureName->ReadOnly = true;
            this->textureName->Width = 250;
            // 
            // textureStreamable
            // 
            this->textureStreamable->HeaderText = L"Streamable";
            this->textureStreamable->Name = L"textureStreamable";
            this->textureStreamable->ReadOnly = true;
            // 
            // filterTimer
            // 
            this->filterTimer->Interval = 1000;
            this->filterTimer->Tick += gcnew System::EventHandler(this, &TexturesDatabaseViewer::filterTimer_Tick);
            // 
            // TexturesDatabaseViewer
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(649, 478);
            this->Controls->Add(this->tableLayoutPanel1);
            this->Name = L"TexturesDatabaseViewer";
            this->Text = L"TexturesDatabaseViewer";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->texturesDataSet))->EndInit();
            this->tableLayoutPanel1->ResumeLayout(false);
            this->tableLayoutPanel1->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView))->EndInit();
            this->ResumeLayout(false);

        }

#pragma endregion

    public:
        void SetDataProvider(MyDict<HashString, MetroTextureInfo*>* data);
        void SetMainForm(MainForm^ form);
        void FillWithData();
    private:
        MyDict<HashString, MetroTextureInfo*>* mDataProvider;
        MainForm^ mMainForm;
        array<String^>^ mFileExtensions;
        void filterTimer_Tick(System::Object^ sender, System::EventArgs^ e);
        void filterText_TextChanged(System::Object^ sender, System::EventArgs^ e);
        void dataGridView_MouseDoubleClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
};
}
