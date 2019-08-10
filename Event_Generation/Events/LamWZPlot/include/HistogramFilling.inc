// Filling the Histogram
        for (int i = 0; i < bn->nvar; ++i)
        {
            if(bn->TwoDQ[i]==0)
            {
                valueX=bn->getValX(i);
                // printf("Value %f\n", value);
                if (SorB==1)
                {
                    bid=Hsig[id][i]->Fill(valueX);
                }
                else
                {
                    bid=Hbkg[id][i]->Fill(valueX);
                }
            }
            else
            {
                valueX=bn->getValX(i);
                valueY=bn->getValY(i);
                // printf("Value %f vs %f\n", valueX,valueY);
                if (SorB==1)
                {
                    bid=Hsig2[id][i]->Fill(valueX,valueY);
                }
                else
                {
                    bid=Hbkg2[id][i]->Fill(valueX,valueY);
                }
            }
        }