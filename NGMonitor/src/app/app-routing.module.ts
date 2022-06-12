import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { GraphAppComponent } from './graph-app/graph-app.component';

const routes: Routes = [
  {path: 'graph', component: GraphAppComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
