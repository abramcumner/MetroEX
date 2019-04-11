#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace MetroEX {

    /// <summary>
    /// Summary for TreeViewFilter
    /// </summary>
    public ref class TreeViewFilter : public System::Windows::Forms::UserControl
    {
    public:
        TreeViewFilter(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //

            if (this->filterTimeout != 0) {
                this->filterTimer->Interval = this->filterTimeout;
            }
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~TreeViewFilter()
        {
            if (components)
            {
                delete components;
            }
        }

    public:
        System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel;
        System::Windows::Forms::TextBox^  filterTextBox;
        System::Windows::Forms::TreeView^  treeView;
        System::Windows::Forms::Timer^  filterTimer;

        int filterTimeout;
    private: System::ComponentModel::IContainer^  components;
    public:


    protected:

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
            this->tableLayoutPanel = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->filterTextBox = (gcnew System::Windows::Forms::TextBox());
            this->treeView = (gcnew System::Windows::Forms::TreeView());
            this->filterTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->tableLayoutPanel->SuspendLayout();
            this->SuspendLayout();
            // 
            // tableLayoutPanel
            // 
            this->tableLayoutPanel->ColumnCount = 1;
            this->tableLayoutPanel->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle()));
            this->tableLayoutPanel->Controls->Add(this->filterTextBox, 0, 0);
            this->tableLayoutPanel->Controls->Add(this->treeView, 0, 1);
            this->tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel->Location = System::Drawing::Point(0, 0);
            this->tableLayoutPanel->Name = L"tableLayoutPanel";
            this->tableLayoutPanel->RowCount = 2;
            this->tableLayoutPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));
            this->tableLayoutPanel->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
            this->tableLayoutPanel->Size = System::Drawing::Size(490, 520);
            this->tableLayoutPanel->TabIndex = 0;
            // 
            // filterTextBox
            // 
            this->filterTextBox->Dock = System::Windows::Forms::DockStyle::Fill;
            this->filterTextBox->Location = System::Drawing::Point(3, 3);
            this->filterTextBox->Name = L"filterTextBox";
            this->filterTextBox->Size = System::Drawing::Size(484, 20);
            this->filterTextBox->TabIndex = 0;
            this->filterTextBox->TextChanged += gcnew System::EventHandler(this, &TreeViewFilter::filterText_TextChanged);
            // 
            // treeView
            // 
            this->treeView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->treeView->Location = System::Drawing::Point(3, 28);
            this->treeView->Name = L"treeView";
            this->treeView->Size = System::Drawing::Size(484, 489);
            this->treeView->TabIndex = 1;
            // 
            // filterTimer
            // 
            this->filterTimer->Interval = 1000;
            this->filterTimer->Tick += gcnew System::EventHandler(this, &TreeViewFilter::filterTimer_Tick);
            // 
            // TreeViewFilter
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->tableLayoutPanel);
            this->Name = L"TreeViewFilter";
            this->Size = System::Drawing::Size(490, 520);
            this->tableLayoutPanel->ResumeLayout(false);
            this->tableLayoutPanel->PerformLayout();
            this->ResumeLayout(false);

        }
#pragma endregion

        private:
            array<TreeNode^>^ mOriginalRootNodes;
            void filterTimer_Tick(System::Object^ sender, System::EventArgs^ e);
            void filterText_TextChanged(System::Object^ sender, System::EventArgs^ e);
            void FilterTreeView(TreeNode^ node, String^ text);
            void Initialize();

            property TreeView^ DataTree {
                TreeView^ get() { return this->treeView; }
            }
    };
}
