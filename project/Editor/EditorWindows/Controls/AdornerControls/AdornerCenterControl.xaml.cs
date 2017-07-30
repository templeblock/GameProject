﻿using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace Editor.EditorWindows.Controls.AdornerControls
{
    /// <summary>
    /// Interaction logic for AdornerCenterControl.xaml
    /// </summary>
    public partial class AdornerCenterControl : UserControl
    {
        public AdornerCenterControl()
        {
            InitializeComponent();
        }

        public enum dropLocation
        {
            none,
            center,
            top,
            bottom,
            left,
            right
        }

        private Rectangle rect = null;

        public dropLocation GetDrop()
        {
            if (pRect == center)
            {
                return dropLocation.center;
            }
            else if (pRect == top)
            {
                return dropLocation.top;
            }
            else if (pRect == bottom)
            {
                return dropLocation.bottom;
            }
            else if (pRect == left)
            {
                return dropLocation.left;
            }
            else if (pRect == right)
            {
                return dropLocation.right;
            }

            return dropLocation.none;
        }

        private Rectangle pRect = null;

        private void Rectangle_MouseEnter(object sender, MouseEventArgs e)
        {
            double w = 0;
            double h = 0;

            VerticalAlignment va = VerticalAlignment.Stretch;
            HorizontalAlignment ha = HorizontalAlignment.Stretch;

            pRect = sender as Rectangle;
            if (sender as Rectangle == center)
            {
                w = ActualWidth;
                h = ActualHeight;
            }
            else if (sender as Rectangle == top)
            {
                w = ActualWidth;
                h = ActualHeight / 2.0;
                va = VerticalAlignment.Top;
            }
            else if (sender as Rectangle == bottom)
            {
                w = ActualWidth;
                h = ActualHeight / 2.0;
                va = VerticalAlignment.Bottom;
            }
            else if (sender as Rectangle == left)
            {
                w = ActualWidth / 2.0;
                h = ActualHeight;
                ha = HorizontalAlignment.Left;
            }
            else if (sender as Rectangle == right)
            {
                w = ActualWidth / 2.0;
                h = ActualHeight;
                ha = HorizontalAlignment.Right;
            }

            if (rect == null)
            {
                rect = new Rectangle();
                Color c = new Color();
                c.A = 0x33;
                c.R = 0x00;
                c.G = 0x00;
                c.B = 0xFF;
                rect.Fill = new SolidColorBrush(c);
                rect.Width = w;
                rect.Height = h;
                rect.VerticalAlignment = va;
                rect.HorizontalAlignment = ha;
                rect.IsHitTestVisible = false;

                grid.Children.Add(rect);
            }

            rect.Width = w;
            rect.Height = h;
            rect.VerticalAlignment = va;
            rect.HorizontalAlignment = ha;
        }

        private void Rectangle_MouseLeave(object sender, MouseEventArgs e)
        {
            if (rect != null)
            {
                grid.Children.Remove(rect);
                rect = null;
            }
        }

        private void Rectangle_MouseMove(object sender, MouseEventArgs e)
        {
            //pRect = rect;
        }
    }
}